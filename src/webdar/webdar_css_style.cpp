    // C system header files
extern "C"
{

}

    // C++ system header files


    // webdar headers
#include "tokens.hpp"


    //
#include "webdar_css_style.hpp"

using namespace std;


void webdar_style_normal_button(html_button & obj)
{
    css tmp_set;
    html_url_class uclass;

    obj.css_border_style(css::bd_all, css::bd_solid, true);
    obj.css_border_width(css::bd_all, css::bd_medium, true);
    obj.css_width("8em", true, true);
    obj.css_padding("0.5em", true);
    obj.css_margin("0.2em", true);
    obj.css_text_align(al_center, true);

    obj.css_background_color(COLOR_MENU_BACK_OFF, true);
    obj.css_border_color(css::bd_all, COLOR_MENU_BORDER_OFF, true);

    tmp_set.css_color(COLOR_MENU_FRONT_OFF, true);
    tmp_set.css_font_weight_bold(true);
    tmp_set.css_font_style_italic(true);
    tmp_set.css_text_decoration(css::dc_none, true);
    uclass.set_style_link(tmp_set);
    uclass.set_style_visited(tmp_set);

    tmp_set.css_color(COLOR_MENU_FRONT_HOVER_OFF, true);
    tmp_set.css_text_decoration(dc_underline, true);
    uclass.set_style_hover(tmp_set);

    tmp_set.css_color(COLOR_MENU_FRONT_ACTIVE_OFF, true);
    uclass.set_style_active(tmp_set);

    obj.set_url_class(uclass);

    css tmp_set2;
    tmp_set2.css_border_style(css::bd_all, css::bd_none);
    obj.set_url_css(tmp_set2);
}

void webdar_style_active_button(html_button & obj)
{
    css tmp_set;
    html_url_class uclass;

    obj.css_border_style(css::bd_all, css::bd_solid, true);
    obj.css_border_width(css::bd_all, css::bd_medium, true);
    obj.css_width("8em", true, true);
    obj.css_padding("0.5em", true);
    obj.css_margin("0.2em", true);
    obj.css_text_align(al_center, true);

    obj.css_background_color(COLOR_MENU_BACK_ON, true);
    obj.css_border_color(css::bd_all, COLOR_MENU_BORDER_ON, true);

    tmp_set.css_color(COLOR_MENU_FRONT_ON, true);
    tmp_set.css_font_weight_bold(true);
    tmp_set.css_font_style_normal(true);
    tmp_set.css_text_decoration(css::dc_none, true);
    uclass.set_style_link(tmp_set);
    uclass.set_style_visited(tmp_set);

    tmp_set.css_color(COLOR_MENU_FRONT_HOVER_ON, true);
    tmp_set.css_text_decoration(dc_underline, true);
    uclass.set_style_hover(tmp_set);

    tmp_set.css_color(COLOR_MENU_FRONT_ACTIVE_ON, true);
    uclass.set_style_active(tmp_set);

    obj.set_url_class(uclass);

    css tmp_set2;
    tmp_set2.css_border_style(css::bd_all, css::bd_none);
    obj.set_url_css(tmp_set2);

}

void webdar_style_void_button(html_button & obj)
{
    obj.css_border_style(css::bd_all, css::bd_none, true);
    obj.css_border_width(css::bd_all, css::bd_medium, true);
    obj.css_width("8em", true, true);
    obj.css_padding("0.5em", true);
    obj.css_margin("0.2em", true);
    obj.css_text_align(al_center, true);

}
