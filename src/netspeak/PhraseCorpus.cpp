#include "netspeak/PhraseCorpus.hpp"

#include <aio.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <string>
#include <utility>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/lexical_cast.hpp>

#include "aitools/invertedindex/ByteBuffer.hpp"
#include "aitools/util/check.hpp"

#include "netspeak/error.hpp"
#include "netspeak/phrase_methods.hpp"
#include "netspeak/query_methods.hpp"

namespace netspeak {

namespace bfs = boost::filesystem;

const std::string PhraseCorpus::txt_dir("txt");

const std::string PhraseCorpus::bin_dir("bin");

const std::string PhraseCorpus::vocab_file("vocab");

const std::string PhraseCorpus::phrase_file("phrases");

PhraseCorpus::PhraseCorpus() {}

PhraseCorpus::PhraseCorpus(const bfs::path& phrase_dir) {
  open(phrase_dir);
}

PhraseCorpus::~PhraseCorpus() {
  close();
}

void PhraseCorpus::close() {
  for (const auto& pair : phrase_len_to_file_des_) {
    ::close(pair.second);
  }
}

size_t PhraseCorpus::count_phrases(size_t phrase_len) const {
  size_t count(0);
  const auto it_fd = phrase_len_to_file_des_.find(phrase_len);
  const auto it_size = phrase_len_to_encoded_size_.find(phrase_len);
  if (it_fd != phrase_len_to_file_des_.end() &&
      it_size != phrase_len_to_encoded_size_.end()) {
    struct stat stats;
    ::fstat(it_fd->second, &stats);
    count = stats.st_size / it_size->second;
  }
  return count;
}

size_t PhraseCorpus::count_vocabulary() const {
  return word_id_to_word_str_.size();
}

bool PhraseCorpus::is_open() const {
  return !word_id_to_word_str_.empty();
}

void PhraseCorpus::open(const bfs::path& phrase_dir) {
  init_vocabulary_(phrase_dir / vocab_file);
  open_phrase_files_(phrase_dir);
}
generated::Phrase PhraseCorpus::read_phrase(size_t phrase_len,
                                            size_t phrase_id) const {
  const auto size_it = phrase_len_to_encoded_size_.find(phrase_len);
  aitools::check(size_it != phrase_len_to_encoded_size_.end(), __func__);
  const auto fd_it = phrase_len_to_file_des_.find(phrase_len);
  aitools::check(fd_it != phrase_len_to_file_des_.end(), __func__);
  aitools::invertedindex::ByteBuffer buffer(size_it->second);
  const uint64_t offset = phrase_id * size_it->second;
  aitools::check(
      ::pread(fd_it->second, static_cast<void*>(buffer.begin()), buffer.size(),
              offset) == static_cast<ssize_t>(size_it->second),
      __func__, "pread64 failed");
  return decode_(buffer, phrase_id);
}

generated::Phrase PhraseCorpus::read_phrase(
    const generated::QueryResult::PhraseRef& phrase_ref) const {
  return read_phrase(phrase_ref.length(), phrase_ref.id());
}

std::vector<generated::Phrase> PhraseCorpus::read_phrases(
    const std::vector<std::pair<size_t, size_t> >& refs) const {
  const size_t aio_count = refs.size();
  std::vector<aitools::invertedindex::ByteBuffer> buffers;
  buffers.reserve(aio_count); // Do not use the c'tor: buffers(aio_count),
  // b/c ByteBuffer copies are shallow by default.
  std::vector<aiocb> aio_read_objs(aio_count);
  std::vector<aiocb*> aio_read_ptrs(aio_count);
  std::memset(aio_read_objs.data(), 0, aio_count * sizeof(aiocb));
  for (unsigned i = 0; i != aio_count; ++i) {
    const auto ref = refs[i];
    const auto size_it = phrase_len_to_encoded_size_.find(ref.first);
    aitools::check(size_it != phrase_len_to_encoded_size_.end(), __func__);
    const auto fd_it = phrase_len_to_file_des_.find(ref.first);
    aitools::check(fd_it != phrase_len_to_file_des_.end(), __func__);

    buffers.push_back(aitools::invertedindex::ByteBuffer(size_it->second));
    aio_read_objs[i].aio_buf = buffers.back().begin();
    aio_read_objs[i].aio_nbytes = size_it->second;
    aio_read_objs[i].aio_fildes = fd_it->second;
    aio_read_objs[i].aio_offset = ref.second * size_it->second;
    aio_read_objs[i].aio_lio_opcode = LIO_READ;
    aio_read_ptrs[i] = &aio_read_objs[i];
  }
  aitools::check(
      ::lio_listio(LIO_WAIT, aio_read_ptrs.data(), aio_count, NULL) != -1,
      __func__, "lio_listio failed");
  std::vector<generated::Phrase> phrases;
  phrases.reserve(aio_count);
  for (unsigned i = 0; i != aio_count; ++i) {
    aitools::check(::aio_return(aio_read_ptrs[i]) ==
                       static_cast<ssize_t>(aio_read_ptrs[i]->aio_nbytes),
                   __func__, "aio_read64 failed");
    phrases.push_back(decode_(buffers[i], refs[i].second));
  }
  return phrases;
}

std::vector<generated::Phrase> PhraseCorpus::read_phrases(
    const std::vector<generated::QueryResult::PhraseRef>& phrase_refs) const {
  std::vector<std::pair<size_t, size_t> > refs(phrase_refs.size());
  for (unsigned i = 0; i != refs.size(); ++i) {
    refs[i].first = phrase_refs[i].length();
    refs[i].second = phrase_refs[i].id();
  }
  return read_phrases(refs);
}

generated::Phrase PhraseCorpus::decode_(
    aitools::invertedindex::ByteBuffer& buffer, size_t phrase_id) const {
  generated::Phrase phrase;
  PhraseCorpusPhraseFreq phrase_freq;
  if (buffer.get(phrase_freq)) {
    phrase.set_frequency(phrase_freq);
    WordId word_id;
    while (buffer.get(word_id)) {
      phrase.add_word()->set_text(word_id_to_word_str_.at(word_id));
    }
  }
  phrase.set_id(make_unique_id(phrase.word_size(), phrase_id));
  return phrase;
}

void PhraseCorpus::init_vocabulary_(const bfs::path& vocab_file) {
  bfs::ifstream ifs(vocab_file);
  aitools::check(ifs.is_open(), error_message::cannot_open, vocab_file);

  std::string word;
  WordId word_id;
  while (ifs >> word >> word_id) {
    word_id_to_word_str_[word_id] = word;
  }
}

void PhraseCorpus::open_phrase_files_(const bfs::path& phrase_dir) {
  bfs::directory_iterator end;
  for (bfs::directory_iterator it(phrase_dir); it != end; ++it) {
    if (boost::starts_with(it->path().filename().string(), phrase_file)) {
      std::string::size_type dotpos = it->path().filename().string().rfind('.');
      if (dotpos == std::string::npos)
        continue;
      const size_t phrase_len = boost::lexical_cast<size_t>(
          it->path().filename().string().substr(++dotpos));
      phrase_len_to_file_des_.insert(std::make_pair(
          phrase_len, ::open(it->path().string().c_str(), O_RDONLY)));
      aitools::check(phrase_len_to_file_des_[phrase_len] != -1,
                     error_message::cannot_open, it->path());
      phrase_len_to_encoded_size_[phrase_len] =
          sizeof(PhraseCorpusPhraseFreq) + sizeof(PhraseId) * phrase_len;
    }
  }
}

} // namespace netspeak
