/* This file is part of SEAL-TK, and is distributed under the OSI-approved BSD
 * 3-Clause License. See top-level LICENSE file or
 * https://github.com/Kitware/seal-tk/blob/master/LICENSE for details. */

#include <vital/algo/algorithm_factory.h>
#include <vital/algo/image_io.h>
#include <vital/plugin_loader/plugin_manager.h>

#include <QRegularExpression>

namespace kv = kwiver::vital;

namespace sealtk
{

namespace core
{

namespace test
{

// ============================================================================
class TimestampPassthrough : public kv::algorithm_impl<TimestampPassthrough,
  kv::algo::image_io>
{
public:
  TimestampPassthrough();

  ~TimestampPassthrough() override = default;

  kv::config_block_sptr get_configuration() const
    override;

  void set_configuration(kv::config_block_sptr config) override;

  bool check_configuration(kv::config_block_sptr config) const override;

private:
  kv::algo::image_io_sptr imageReader;

  kv::image_container_sptr load_(std::string const& filename) const override;

  void save_(std::string const& filename, kv::image_container_sptr data) const
    override;

  kv::metadata_sptr load_metadata_(std::string const& filename) const override;

  kv::metadata_sptr fixupMetadata(std::string const& filename,
                                  kv::metadata_sptr md) const;
};

// ----------------------------------------------------------------------------
TimestampPassthrough::TimestampPassthrough()
{
  this->set_capability(kv::algo::image_io::HAS_TIME, true);
}

// ----------------------------------------------------------------------------
kv::config_block_sptr TimestampPassthrough::get_configuration() const
{
  auto config = kv::algo::image_io::get_configuration();

  kv::algo::image_io::get_nested_algo_configuration("image_reader", config,
    this->imageReader);

  return config;
}

// ----------------------------------------------------------------------------
void TimestampPassthrough::set_configuration(kv::config_block_sptr config)
{
  auto newConfig = this->get_configuration();
  newConfig->merge_config(config);

  kv::algo::image_io::set_nested_algo_configuration("image_reader", newConfig,
    this->imageReader);
}

// ----------------------------------------------------------------------------
bool TimestampPassthrough::check_configuration(kv::config_block_sptr config)
  const
{
  return kv::algo::image_io::check_nested_algo_configuration("image_reader",
    config);
}

// ----------------------------------------------------------------------------
kv::image_container_sptr TimestampPassthrough::load_(
  std::string const& filename) const
{
  if (this->imageReader)
  {
    auto im = this->imageReader->load(filename);
    im->set_metadata(this->fixupMetadata(filename, im->get_metadata()));
    return im;
  }

  return nullptr;
}

// ----------------------------------------------------------------------------
void TimestampPassthrough::save_(std::string const& filename,
                                 kv::image_container_sptr data) const
{
  if (this->imageReader)
  {
    this->imageReader->save(filename, data);
  }
}

// ----------------------------------------------------------------------------
kv::metadata_sptr TimestampPassthrough::load_metadata_(
  std::string const& filename) const
{
  if (this->imageReader)
  {
    return this->fixupMetadata(filename,
                               this->imageReader->load_metadata(filename));
  }

  return this->fixupMetadata(filename, nullptr);
}

// ----------------------------------------------------------------------------
kv::metadata_sptr TimestampPassthrough::fixupMetadata(
  std::string const& filename, kv::metadata_sptr md) const
{
  static QRegularExpression const regex{".*[^0-9]([0-9]+)\\.png"};

  if (!md)
  {
    md = std::make_shared<kv::metadata>();
  }

  auto match = regex.match(QString::fromStdString(filename));
  if (match.hasMatch())
  {
    std::istringstream is{match.captured(1).toStdString()};
    kv::timestamp::time_t time;
    is >> time;
    kv::timestamp ts;
    ts.set_time_usec(time);
    md->set_timestamp(ts);
  }

  return md;
}

// ----------------------------------------------------------------------------
void loadKwiverPlugins()
{
  kv::plugin_manager::instance().load_all_plugins();
  kv::plugin_manager::instance().ADD_ALGORITHM("timestamp_passthrough",
                                               TimestampPassthrough);
}

}

}

}
