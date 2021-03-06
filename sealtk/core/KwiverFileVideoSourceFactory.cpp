/* This file is part of SEAL-TK, and is distributed under the OSI-approved BSD
 * 3-Clause License. See top-level LICENSE file or
 * https://github.com/Kitware/seal-tk/blob/master/LICENSE for details. */

#include <sealtk/core/KwiverFileVideoSourceFactory.hpp>

#include <sealtk/core/KwiverVideoSource.hpp>
#include <sealtk/core/VideoController.hpp>

#include <vital/algo/video_input.h>

namespace sealtk
{

namespace core
{

// ============================================================================
class KwiverFileVideoSourceFactoryPrivate
{
public:
};

// ----------------------------------------------------------------------------
QTE_IMPLEMENT_D_FUNC(KwiverFileVideoSourceFactory)

// ----------------------------------------------------------------------------
KwiverFileVideoSourceFactory::KwiverFileVideoSourceFactory(
  VideoController* parent)
  : FileVideoSourceFactory{parent},
    d_ptr{new KwiverFileVideoSourceFactoryPrivate}
{
}

// ----------------------------------------------------------------------------
KwiverFileVideoSourceFactory::~KwiverFileVideoSourceFactory()
{
}

// ----------------------------------------------------------------------------
void KwiverFileVideoSourceFactory::loadFile(void* handle, QString const& path)
{
  kwiver::vital::algo::video_input_sptr vi;
  kwiver::vital::algo::video_input::set_nested_algo_configuration(
    "video_reader", this->config(path), vi);
  vi->open(path.toStdString());

  auto* vs = new KwiverVideoSource{this->videoController()};
  vs->setVideoInput(vi);
  this->videoController()->addVideoSource(vs);
  emit this->videoSourceLoaded(handle, vs);
}

}

}
