/***************************************************************************
 *   Copyright (c) 2005 Werner Mayer <wmayer[at]users.sourceforge.net>     *
 *                                                                         *
 *   This file is part of the FreeCAD CAx development system.              *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License as published by the Free Software Foundation; either          *
 *   version 2 of the License, or (at your option) any later version.      *
 *                                                                         *
 *   This library  is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Library General Public License for more details.                  *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this library; see the file COPYING.LIB. If not,    *
 *   write to the Free Software Foundation, Inc., 59 Temple Place,         *
 *   Suite 330, Boston, MA  02111-1307, USA                                *
 *                                                                         *
 ***************************************************************************/


#include "PreCompiled.h"

#include "DlgSettingsImageImp.h"
#include "SpinBox.h"

using namespace Gui::Dialog;
using namespace std;

/* TRANSLATOR Gui::Dialog::DlgSettingsImageImp */

/**
 *  Constructs a DlgSettingsImageImp as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
DlgSettingsImageImp::DlgSettingsImageImp( QWidget* parent )
  : QWidget( parent )
{
    this->setupUi(this);
    SbVec2s res = SoOffscreenRenderer::getMaximumResolution();
    spinWidth->setMaximum((int)res[0]);
    spinHeight->setMaximum((int)res[1]);

    _width = width();
    _height = height();
    _fRatio = (float)_width/(float)_height;
}

/**
 *  Destroys the object and frees any allocated resources
 */
DlgSettingsImageImp::~DlgSettingsImageImp()
{
    // no need to delete child widgets, Qt does it all for us
}

void DlgSettingsImageImp::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange) {
        this->retranslateUi(this);
    }
    QWidget::changeEvent(e);
}

/**
 * Sets the image size to (\a w, \a h).
 */
void DlgSettingsImageImp::setImageSize(int w, int h)
{
    // set current screen size
    standardSizeBox->setItemData(0, QSize(w,h));

    spinWidth->setValue(w);
    spinHeight->setValue(h);

    // As the image size is in pixel why shouldn't _width and _height be integers?
    _width  = w;
    _height = h;
    _fRatio = (float)_width/(float)_height;
}

/**
 * Sets the image size to \a s.
 */
void DlgSettingsImageImp::setImageSize( const QSize& s )
{
    // set current screen size
    standardSizeBox->setItemData(0, s);

    spinWidth->setValue( s.width() );
    spinHeight->setValue( s.height() );

    // As the image size is in pixel why shouldn't _width and _height be integers?
    _width  = s.width();
    _height = s.height();
    _fRatio = (float)_width/(float)_height;
}

/**
 * Returns the currently set image size.
 */
QSize DlgSettingsImageImp::imageSize() const
{
    return QSize( spinWidth->value(), spinHeight->value() );
}

/**
 * Returns the currently set image width.
 */
int DlgSettingsImageImp::imageWidth() const
{
    return spinWidth->value();
}

/**
 * Returns the currently set image height.
 */
int DlgSettingsImageImp::imageHeight() const
{
    return spinHeight->value();
}

/**
 * Returns the comment of the picture. If for the currently selected image format no comments are supported
 * QString::null is returned.
 */
QString DlgSettingsImageImp::comment() const
{
    if ( !textEditComment->isEnabled() )
        return QString::null;
    else
        return textEditComment->toPlainText();
}

int DlgSettingsImageImp::backgroundType() const
{
    return comboBackground->currentIndex();
}

/**
 * Sets the image size to (\a w, \a h).
 */
void DlgSettingsImageImp::setBackgroundType(int t)
{
    if ( t < comboBackground->count() )
        comboBackground->setCurrentIndex(t);
}

bool DlgSettingsImageImp::addWatermark() const
{
    return checkWatermark->isChecked();
}

void DlgSettingsImageImp::onSelectedFilter(const QString& filter)
{
    bool ok = (filter.startsWith(QLatin1String("JPG")) ||
               filter.startsWith(QLatin1String("JPEG")) ||
               filter.startsWith(QLatin1String("PNG")));
    buttonGroupComment->setEnabled( ok );
}

void DlgSettingsImageImp::adjustImageSize(float fRatio)
{
    // if width has changed then adjust height and vice versa, if both has changed then adjust width
    if (_height != spinHeight->value())
    {
        _height = spinHeight->value();
        _width = (int)((float)_height*fRatio);
        spinWidth->setValue( (int)_width );
    }
    else // if( _width != spinWidth->value() )
    {
        _width = spinWidth->value();
        _height = (int)((float)_width/fRatio);
        spinHeight->setValue( (int)_height );
    }
}

void DlgSettingsImageImp::on_buttonRatioScreen_clicked()
{
    adjustImageSize(_fRatio);
}

void DlgSettingsImageImp::on_buttonRatio4x3_clicked()
{
    adjustImageSize(4.0f/3.0f);
}

void DlgSettingsImageImp::on_buttonRatio16x9_clicked()
{
    adjustImageSize(16.0f/9.0f);
}

void DlgSettingsImageImp::on_buttonRatio1x1_clicked()
{
    adjustImageSize(1.0f);
}

void DlgSettingsImageImp::on_standardSizeBox_activated(int index)
{
    if (index == 0) {
        // we have set the user data for the 1st item
        QSize s = standardSizeBox->itemData(0).toSize();
        spinWidth->setValue(s.width());
        spinHeight->setValue(s.height());
    }
    else {
        // try to extract from the string
        QString text = standardSizeBox->itemText(index);
        QRegExp rx(QLatin1String("\\b\\d{2,4}\\b"));
        int pos = 0;
        pos = rx.indexIn(text, pos);
        QString w = text.mid(pos, rx.matchedLength());
        spinWidth->setValue(w.toInt());
        pos += rx.matchedLength();
        pos = rx.indexIn(text, pos);
        QString h = text.mid(pos, rx.matchedLength());
        spinHeight->setValue(h.toInt());
    }
}

#include "moc_DlgSettingsImageImp.cpp"
