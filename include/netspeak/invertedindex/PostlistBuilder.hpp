// PostlistBuilder.hpp -*- C++ -*-
// Copyright (C) 2011-2013 Martin Trenkmann
#ifndef NETSPEAK_INVERTEDINDEX_POSTLIST_BUILDER_HPP
#define NETSPEAK_INVERTEDINDEX_POSTLIST_BUILDER_HPP

#include <format>
#include <limits>
#include <memory>
#include <netspeak/util/std_path_formatter.hpp>
#include <numeric>
#include <stdexcept>

#include <boost/utility.hpp>

#include "../util/logging.hpp"
#include "Iterator.hpp"
#include "Postlist.hpp"

namespace netspeak::invertedindex {

/**
 * A class to build postlists.
 */
template <typename T>
class PostlistBuilder : public boost::noncopyable {
public:
  typedef T value_type;
  typedef value::value_traits<value_type> traits_type;

private:
  typedef std::vector<value_type> value_vector;
  typedef std::vector<uint32_t> size_vector;

public:
  explicit PostlistBuilder(size_t page_size = 10 * 1024 * 1024) : page_size_(page_size), mem_usage_(0) {}

  ~PostlistBuilder() {}

  std::unique_ptr<Postlist<value_type> > build() {
    if (sizes_.empty()) {
      return std::unique_ptr<Postlist<value_type> >(new Postlist<value_type>);
    }

    head_.value_count = sizes_.size();
    head_.value_size = sizes_.front();
    head_.total_size = std::accumulate(sizes_.begin(), sizes_.end(), 0);
    for (auto it(sizes_.begin()); it != sizes_.end(); ++it) {
      if (*it != head_.value_size) {
        head_.value_size = 0; // indicates variable value size
        break;
      }
    }

    std::unique_ptr<Postlist<value_type> > postlist;
    if (has_swap_()) {
      util::rewind(swap_.stream_);
      swap_.pagesize_ = page_size_;
      postlist.reset((head_.value_size == 0) ? new Postlist<value_type>(head_, swap_, sizes_)
                                             : new Postlist<value_type>(head_, swap_));
      swap_.stream_ = nullptr; // postlist gets file stream ownership
    } else {
      page_type page(head_.total_size);
      for (auto it(values_.begin()); it != values_.end(); ++it) {
        if (page.buffer_.put(*it))
          continue;
        throw std::runtime_error(std::format("ByteBuffer::put failed : {}", *it));
      }
      assert(page.buffer_.position() == page.buffer_.end());
      postlist.reset((head_.value_size == 0) ? new Postlist<value_type>(head_, page, sizes_)
                                             : new Postlist<value_type>(head_, page));
    }
    clear();
    return postlist;
  }

  void clear() {
    util::fclose(swap_.stream_);
    head_ = Head();
    swap_ = swap_type();
    values_.clear();
    sizes_.clear();
    mem_usage_ = 0;
  }

  void push_back(const value_type& value) {
    sizes_.push_back(traits_type::size_of(value));
    if (has_swap_()) {
      traits_type::write_to(value, swap_.stream_);
    } else {
      values_.push_back(value);
      mem_usage_ += sizes_.back();
      if (mem_usage_ > page_size_) {
        swap_out_values_();
      }
    }
  }

private:
  void swap_out_values_() {
    swap_.stream_ = util::tmpfile();
    for (auto it(values_.begin()); it != values_.end(); ++it) {
      traits_type::write_to(*it, swap_.stream_);
    }
    values_.clear();
    mem_usage_ = 0;
  }

  bool has_swap_() const {
    return swap_.stream_ != nullptr;
  }

private:
  size_t page_size_;
  size_t mem_usage_;
  value_vector values_;
  size_vector sizes_;
  Head head_;
  swap_type swap_;
};

} // namespace netspeak::invertedindex

#endif // NETSPEAK_INVERTEDINDEX_POSTLIST_BUILDER_HPP
