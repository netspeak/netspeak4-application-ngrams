#include <google/protobuf/util/json_util.h>

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <format>
#include <netspeak/error.hpp>
#include <netspeak/service/RequestLogger.hpp>
#include <netspeak/service/tracking.hpp>
#include <netspeak/util/JsonWriter.hpp>
#include <netspeak/util/systemio.hpp>
#include <sstream>


namespace netspeak::service {


/**
 * @brief Returns the current time formatted as a string following the [ISO
 * 8601](https://en.wikipedia.org/wiki/ISO_8601) format for UTC date and time.
 */
static std::string utc_timestamp() {
  const auto now = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now());
  return std::format("{:%FT%TZ}", now);
}

static std::string get_log_file_prefix() {
  /** \todo could we just use the utc_timestamp instead or use - as a separate but keep the decimal point? The latter
   * would only require a different format specifier: `{:%Y-%m-%dT%H-%M-%SZ}`. */
  auto time = netspeak::service::utc_timestamp();
  std::replace(time.begin(), time.end(), ':', '-');
  std::replace(time.begin(), time.end(), '.', '-');
  return time;
}

RequestLogger::RequestLogger(std::unique_ptr<NetspeakService::Service> service, fs::path log_dir)
    : service_(std::move(service)), req_counter_(0) {
  if (!fs::is_directory(log_dir)) {
    fs::create_directories(log_dir);
  }

  util::log("Creating log files in", log_dir);

  const auto prefix = (log_dir / get_log_file_prefix()).string();

  f_search_req_.lock()->open(prefix + "_search_req.jsonl");
  f_search_error.lock()->open(prefix + "_search_error.jsonl");
  f_get_corpora_req_.lock()->open(prefix + "_get_corpora_req.jsonl");
  f_get_corpora_error.lock()->open(prefix + "_get_corpora_error.jsonl");
}

template <class S, class R>
util::JsonWriter::ObjectContinue<S, R> log_boilerplate(util::JsonWriter::Value<S, R> writer, uint64_t req_id,
                                                       const std::string& user,
                                                       const google::protobuf::Message& message) {
  return writer.object()
      .prop("timestamp")
      .string(utc_timestamp())
      .prop("req_id")
      .number(req_id)
      .prop("user")
      .string(user)
      .prop("message")
      .protobuf_message(message);
}
template <class S, class R>
util::JsonWriter::ObjectContinue<S, R> log_error_status(util::JsonWriter::ObjectContinue<S, R> writer,
                                                        const grpc::Status& status) {
  return writer.prop("status")
      .object()
      .prop("code")
      .number(status.error_code())
      .prop("message")
      .string(status.error_message())
      .prop("details")
      .string(status.error_details())
      .endObject();
}

grpc::Status RequestLogger::Search(grpc::ServerContext* context, const SearchRequest* request,
                                   SearchResponse* response) {
  const auto user = get_tracking_id(*context);
  const auto req_id = req_counter_++;

  {
    std::string log_line;
    log_boilerplate(util::JsonWriter::create(log_line), req_id, user, *request).endObject().done();

    {
      auto lock = f_search_req_.lock();
      *lock << log_line << std::endl;
    }
  }

  const auto status = service_->Search(context, request, response);

  if (!status.ok() || response->has_error()) {
    std::string log_line;
    auto writer = log_boilerplate(util::JsonWriter::create(log_line), req_id, user, *request);

    if (!status.ok()) {
      writer = log_error_status(std::move(writer), status);
    }
    if (response->has_error()) {
      writer = writer.prop("error_message").protobuf_message(*response);
    }

    writer.endObject().done();

    {
      auto lock = f_search_error.lock();
      *lock << log_line << std::endl;
    }
  }

  return status;
}

grpc::Status RequestLogger::GetCorpora(grpc::ServerContext* context, const CorporaRequest* request,
                                       CorporaResponse* response) {
  const auto user = get_tracking_id(*context);
  const auto req_id = req_counter_++;

  {
    std::string log_line;
    log_boilerplate(util::JsonWriter::create(log_line), req_id, user, *request).endObject().done();

    {
      auto lock = f_get_corpora_req_.lock();
      *lock << log_line << std::endl;
    }
  }

  const auto status = service_->GetCorpora(context, request, response);

  if (!status.ok()) {
    std::string log_line;
    log_error_status(log_boilerplate(util::JsonWriter::create(log_line), req_id, user, *request), status)
        .endObject()
        .done();

    {
      auto lock = f_get_corpora_error.lock();
      *lock << log_line << std::endl;
    }
  }

  return status;
}


} // namespace netspeak::service
