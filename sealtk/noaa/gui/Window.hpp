/* This file is part of SEAL-TK, and is distributed under the OSI-approved BSD
 * 3-Clause License. See top-level LICENSE file or
 * https://github.com/Kitware/seal-tk/blob/master/LICENSE for details. */

#ifndef sealtk_noaa_gui_Window_hpp
#define sealtk_noaa_gui_Window_hpp

#include <sealtk/noaa/gui/Export.h>

#include <QMainWindow>
#include <QPointF>
#include <qtGlobal.h>

namespace sealtk
{

namespace noaa
{

namespace gui
{

class WindowPrivate;

class SEALTK_NOAA_GUI_EXPORT Window : public QMainWindow
{
  Q_OBJECT

public:
  explicit Window(QWidget* parent = nullptr);
  ~Window() override;

  Q_PROPERTY(float zoom READ zoom WRITE setZoom NOTIFY zoomSet);

  float zoom() const;
  QPointF center() const;

signals:
  void zoomSet(float zoom) const;
  void centerSet(QPointF center) const;

public slots:
  void showAbout();
  void setZoom(float zoom);
  void setCenter(QPointF center);

protected:
  QTE_DECLARE_PRIVATE(Window)

private:
  QTE_DECLARE_PRIVATE_RPTR(Window)
};

}

}

}

#endif
