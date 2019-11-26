/**
 * \file
 *
 * \author Mattia Basaglia
 *
 * \copyright Copyright (C) 2013-2019 Mattia Basaglia
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include <cstring>
#include <algorithm>

#include <QApplication>
#include <QCommandLineParser>

#include "QtColorWidgets/color_2d_slider.hpp"
#include "QtColorWidgets/color_delegate.hpp" /// \todo show it
#include "QtColorWidgets/color_dialog.hpp"
#include "QtColorWidgets/color_line_edit.hpp"
#include "QtColorWidgets/color_list_widget.hpp"
#include "QtColorWidgets/color_palette_widget.hpp"
#include "QtColorWidgets/color_preview.hpp"
#include "QtColorWidgets/color_wheel.hpp"
#include "QtColorWidgets/hue_slider.hpp"
#include "QtColorWidgets/gradient_editor.hpp"

bool run = false;
QStringList just_these;

void screenshot(QWidget& widget, QString name = QString())
{
    if ( name.isEmpty() )
    {
        name = widget.metaObject()->className();
        name.remove("color_widgets::");
    }
    if ( !just_these.isEmpty() && !just_these.contains(name) )
        return;

    widget.setWindowTitle(name);
    QPixmap pic(widget.size());
    widget.render(&pic);
    name += ".png";
    pic.save(name);
    if ( run )
        widget.show();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addPositionalArgument("just_these", "Only these widgets");
    QCommandLineOption run_option("run", "Show widgets instead of saving to file");
    parser.addOption(run_option);

    parser.process(a);
    run = parser.isSet(run_option);
    just_these = parser.positionalArguments();

    QColor demo_color(64,172,143,128);

    color_widgets::ColorPalette palette1;
    color_widgets::ColorPalette palette2;
    int palette_columns = 12;
    palette1.setName("Palette 1");
    palette2.setName("Palette 2");
    palette1.setColumns(palette_columns);
    palette2.setColumns(palette_columns);
    for ( int i = 0; i < 6; i++ )
    {
        for ( int j = 0; j < palette_columns; j++ )
        {
            float f = float(j)/palette_columns;
            palette1.appendColor(QColor::fromHsvF(i/8.0,1-f,0.5+f/2));
            palette2.appendColor(QColor::fromHsvF(i/8.0,1-f,1-f));
        }
    }
    color_widgets::ColorPaletteModel palette_model;
    palette_model.addPalette(palette1, false);
    palette_model.addPalette(palette2, false);


    color_widgets::ColorPreview preview;
    preview.setColor(demo_color);
    preview.setDisplayMode(color_widgets::ColorPreview::SplitAlpha);
    preview.resize(128,32);
    screenshot(preview);

    color_widgets::ColorDialog dialog;
    dialog.setColor(demo_color);
    screenshot(dialog);

    color_widgets::Color2DSlider slider2d;
    slider2d.setColor(demo_color);
    slider2d.resize(128,192);
    screenshot(slider2d);

    color_widgets::ColorLineEdit line_edit;
    line_edit.setColor(demo_color);
    line_edit.resize(line_edit.sizeHint());
    screenshot(line_edit);
    line_edit.setPreviewColor(true);
    screenshot(line_edit, "ColorLineEdit_with_color");

    color_widgets::ColorWheel wheel;
    wheel.resize(256, 256);
    wheel.setColor(demo_color);
    screenshot(wheel);

    color_widgets::Swatch swatch;
    swatch.setPalette(palette1);
    swatch.resize(swatch.sizeHint());
    screenshot(swatch);

    color_widgets::ColorPaletteWidget palette_widget;
    palette_widget.setModel(&palette_model);
    screenshot(palette_widget);
    palette_widget.setReadOnly(true);
    screenshot(palette_widget, "ColorPaletteWidget_readonly");

    color_widgets::HueSlider hue_slider;
    hue_slider.setColor(demo_color);
    hue_slider.resize(192, hue_slider.sizeHint().height());
//     hue_slider.setInvertedAppearance(true);
//     hue_slider.setOrientation(Qt::Vertical);
    screenshot(hue_slider);

    color_widgets::ColorListWidget list_widget;
    list_widget.setColors({
        demo_color,
        palette1.colorAt(palette_columns*0),
        palette1.colorAt(palette_columns*1),
        palette1.colorAt(palette_columns*3),
        palette1.colorAt(palette_columns*5),
    });
    list_widget.resize(list_widget.sizeHint());
    screenshot(list_widget);

    color_widgets::GradientEditor editor;
    QGradientStops gradient_colors;
    float n_colors = 6;
    for ( int i = 0; i <= n_colors; ++i )
        gradient_colors.append(QGradientStop(i/n_colors, QColor::fromHsvF(i/n_colors, 0.5, 1)));
    editor.setStops(gradient_colors);
    screenshot(editor);

    if ( run )
        return a.exec();

    return 0;
}
