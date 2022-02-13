#ifndef NK_WIDGET_H
#define NK_WIDGET_H

#define NK_STYLE_WINDOW(ctx){\
	/* window */\
	ctx.style.window.background = nk_rgb(40, 42, 46);\
	ctx.style.window.fixed_background.data.color = nk_rgb(40, 42, 46);\
	ctx.style.window.border_color            = nk_rgb(64,67,67);\
	ctx.style.window.combo_border_color      = nk_rgb(64,67,67);\
	ctx.style.window.contextual_border_color = nk_rgb(64,67,67);\
	ctx.style.window.menu_border_color       = nk_rgb(64,67,67);\
	ctx.style.window.group_border_color      = nk_rgb(64,67,67);\
	ctx.style.window.tooltip_border_color    = nk_rgb(64,67,67);\
	ctx.style.window.scrollbar_size          = nk_vec2(16,16);\
	ctx.style.window.border_color            = nk_rgba(0,0,0,0);\
	ctx.style.window.padding                 = nk_vec2(8,4);\
	ctx.style.window.border                  = 3;\
	\
	/* window header */\
	ctx.style.window.header.normal.data.color = nk_rgb(29, 31, 33);\
	ctx.style.window.header.hover.data.color = nk_rgb(29, 31, 33);\
	ctx.style.window.header.active.data.color = nk_rgb(29, 31, 33);\
	ctx.style.window.header.label_normal = nk_rgb(95,95,95);\
	ctx.style.window.header.label_hover = nk_rgb(95,95,95);\
	ctx.style.window.header.label_active = nk_rgb(95,95,95);\
	/* default text */\
	ctx.style.text.color = nk_rgb(95,95,95);\
}\

#define NK_STYLE_SCROLLBAR(ctx){\
	/* scrollbar */\
	ctx.style.scrollv.normal          = nk_style_item_color(nk_rgb(75,75,75));\
	ctx.style.scrollv.hover           = nk_style_item_color(nk_rgb(85,85,85));\
	ctx.style.scrollv.active          = nk_style_item_color(nk_rgb(85,85,85));\
	ctx.style.scrollv.cursor_normal   = nk_style_item_color(nk_rgb(220,220,220));\
	ctx.style.scrollv.cursor_hover    = nk_style_item_color(nk_rgb(235,235,235));\
	ctx.style.scrollv.cursor_active   = nk_style_item_color(nk_rgb(99,202,255));\
	ctx.style.scrollv.dec_symbol      = NK_SYMBOL_NONE;\
	ctx.style.scrollv.inc_symbol      = NK_SYMBOL_NONE;\
	ctx.style.scrollv.show_buttons    = nk_true;\
	ctx.style.scrollv.border_color    = nk_rgb(81,81,81);\
	ctx.style.scrollv.cursor_border_color = nk_rgb(81,81,81);\
	ctx.style.scrollv.border          = 1;\
	ctx.style.scrollv.rounding        = 0;\
	ctx.style.scrollv.border_cursor   = 1;\
	ctx.style.scrollv.rounding_cursor = 2;\
	\
	/* scrollbar buttons */\
	ctx.style.scrollv.inc_button.normal.data.color = nk_rgba(0, 0, 0, 0);\
	ctx.style.scrollv.inc_button.hover.data.color  = nk_rgba(0, 0, 0, 0);\
	ctx.style.scrollv.inc_button.active.data.color = nk_rgba(0, 0, 0, 0);\
	\
	ctx.style.scrollv.inc_button.border_color    = nk_rgba(0,0,0,0);\
	ctx.style.scrollv.inc_button.text_background = nk_rgba(0,0,0,0);\
	ctx.style.scrollv.inc_button.text_normal     = nk_rgba(0,0,0,0);\
	ctx.style.scrollv.inc_button.text_hover      = nk_rgba(0,0,0,0);\
	ctx.style.scrollv.inc_button.text_active     = nk_rgba(0,0,0,0);\
	ctx.style.scrollv.inc_button.border          = 0.0f;\
	\
	ctx.style.scrollv.dec_button.normal.data.color = nk_rgba(0, 0, 0, 0);\
	ctx.style.scrollv.dec_button.hover.data.color  = nk_rgba(0, 0, 0, 0);\
	ctx.style.scrollv.dec_button.active.data.color = nk_rgba(0, 0, 0, 0);\
	ctx.style.scrollv.dec_button.border_color      = nk_rgba(0,0,0,0);\
	ctx.style.scrollv.dec_button.text_background   = nk_rgba(0,0,0,0);\
	ctx.style.scrollv.dec_button.text_normal       = nk_rgba(0,0,0,0);\
	ctx.style.scrollv.dec_button.text_hover        = nk_rgba(0,0,0,0);\
	ctx.style.scrollv.dec_button.text_active       = nk_rgba(0,0,0,0);\
	ctx.style.scrollv.dec_button.border            = 0.0f;\
}

#define NK_STYLE_CHECKBOX(ctx){\
	/* checkbox toggle */\
	{struct nk_style_toggle *toggle;\
	toggle = &ctx.style.checkbox;\
	toggle->normal.data.color = nk_rgb(75, 75, 75);\
	toggle->hover.data.color = nk_rgb(115, 115, 115);\
	toggle->active.data.color = nk_rgb(25, 25, 25);\
	toggle->cursor_normal.data.color = nk_rgb(95, 95, 95);\
	toggle->cursor_hover.data.color = nk_rgb(95, 95, 95);\
	toggle->text_normal     = nk_rgb(95,95,95);\
	toggle->text_hover      = nk_rgb(95,95,95);\
	toggle->text_active     = nk_rgb(95,95,95);}\
}

#define NK_STYLE_TREE(ctx){\
	/* tree */\
	ctx.style.tab.text = nk_rgb(95,95,95);\
	ctx.style.tab.tab_minimize_button.normal.data.color = nk_rgba(75, 75, 75, 100);\
	ctx.style.tab.tab_minimize_button.hover.data.color = nk_rgba(105, 105, 105, 100);\
	ctx.style.tab.tab_minimize_button.active.data.color = nk_rgba(105, 105, 105, 100);\
	ctx.style.tab.tab_minimize_button.text_background = nk_rgba(0,0,0,0);\
	ctx.style.tab.tab_minimize_button.text_normal = nk_rgba(0,0,0,0);\
	ctx.style.tab.tab_minimize_button.text_hover = nk_rgba(0,0,0,0);\
	ctx.style.tab.tab_minimize_button.text_active = nk_rgba(0,0,0,0);\
\
	ctx.style.tab.tab_maximize_button.normal.data.color = nk_rgba(75, 75, 75, 100);\
	ctx.style.tab.tab_maximize_button.hover.data.color = nk_rgba(105, 105, 105, 100);\
	ctx.style.tab.tab_maximize_button.active.data.color = nk_rgba(105, 105, 105, 100);\
	ctx.style.tab.tab_maximize_button.text_background = nk_rgba(0,0,0,0);\
	ctx.style.tab.tab_maximize_button.text_normal = nk_rgba(0,0,0,0);\
	ctx.style.tab.tab_maximize_button.text_hover = nk_rgba(0,0,0,0);\
	ctx.style.tab.tab_maximize_button.text_active = nk_rgba(0,0,0,0);\
	\
	ctx.style.tab.node_minimize_button.normal.data.color = nk_rgba(75, 75, 75, 100);\
	ctx.style.tab.node_minimize_button.hover.data.color = nk_rgba(105, 105, 105, 100);\
	ctx.style.tab.node_minimize_button.active.data.color = nk_rgba(105, 105, 105, 100);\
	ctx.style.tab.node_minimize_button.text_background = nk_rgba(0,0,0,0);\
	ctx.style.tab.node_minimize_button.text_normal = nk_rgba(0,0,0,0);\
	ctx.style.tab.node_minimize_button.text_hover = nk_rgba(0,0,0,0);\
	ctx.style.tab.node_minimize_button.text_active = nk_rgba(0,0,0,0);\
	\
	ctx.style.tab.node_maximize_button.normal.data.color = nk_rgba(75, 75, 75, 100);\
	ctx.style.tab.node_maximize_button.hover.data.color = nk_rgba(105, 105, 105, 100);\
	ctx.style.tab.node_maximize_button.active.data.color = nk_rgba(105, 105, 105, 100);\
	ctx.style.tab.node_maximize_button.text_background = nk_rgba(0,0,0,0);\
	ctx.style.tab.node_maximize_button.text_normal = nk_rgba(0,0,0,0);\
	ctx.style.tab.node_maximize_button.text_hover = nk_rgba(0,0,0,0);\
	ctx.style.tab.node_maximize_button.text_active = nk_rgba(0,0,0,0);\
}

#define NK_STYLE_TOGGLE(ctx){\
	/* option toggle */\
	{struct nk_style_toggle *toggle;\
	toggle = &ctx.style.option;\
	toggle->normal.data.color        = nk_rgb(95, 95, 95);\
	toggle->hover.data.color         = nk_rgb(115, 115, 115);\
	toggle->active.data.color        = nk_rgb(65, 65, 65);\
	toggle->cursor_normal.data.color = nk_rgb(95, 95, 95);\
	toggle->cursor_hover.data.color  = nk_rgb(95, 95, 95);\
	toggle->text_normal              = nk_rgb(95, 95, 95);\
	toggle->text_hover               = nk_rgb(95, 95, 95);\
	toggle->text_active              = nk_rgb(95, 95, 95);}\
}

#define NK_STYLE_BUTTON(ctx){\
	/* default button */\
	ctx.style.button.normal.data.color   = nk_rgb(75, 75, 75);\
	ctx.style.button.hover.data.color    = nk_rgb(115, 115, 115);\
	ctx.style.button.active.data.color   = nk_rgb(65, 65, 65);\
	ctx.style.button.border_color        = nk_rgba(0,0,0,0);\
	ctx.style.button.text_background     = nk_rgba(0,0,0,0);\
	ctx.style.button.text_normal         = nk_rgb(45,45,45);\
	ctx.style.button.text_hover          = nk_rgb(55,55,55);\
	ctx.style.button.text_active         = nk_rgb(55,55,55);\
	/* contextual button */\
	ctx.style.contextual_button.normal = nk_style_item_color(nk_rgb(206,206,206));\
	ctx.style.contextual_button.hover = nk_style_item_color(nk_rgb(229,229,229));\
	ctx.style.contextual_button.active = nk_style_item_color(nk_rgb(99,202,255));\
	ctx.style.contextual_button.border_color = nk_rgba(0,0,0,0);\
	ctx.style.contextual_button.text_background = nk_rgba(0,0,0,0);\
	ctx.style.contextual_button.text_normal = nk_rgb(95,95,95);\
	ctx.style.contextual_button.text_hover = nk_rgb(95,95,95);\
	ctx.style.contextual_button.text_active = nk_rgb(95,95,95);\
	/* menu button */\
	ctx.style.menu_button.normal = nk_style_item_color(nk_rgb(206,206,206));\
	ctx.style.menu_button.hover = nk_style_item_color(nk_rgb(229,229,229));\
	ctx.style.menu_button.active = nk_style_item_color(nk_rgb(99,202,255));\
	ctx.style.menu_button.border_color = nk_rgba(0,0,0,0);\
	ctx.style.menu_button.text_background = nk_rgba(0,0,0,0);\
	ctx.style.menu_button.text_normal = nk_rgb(95,95,95);\
	ctx.style.menu_button.text_hover = nk_rgb(95,95,95);\
	ctx.style.menu_button.text_active = nk_rgb(95,95,95);\
}

#define NK_STYLE_SELECTABLE(ctx){\
	/* selectable */\
	ctx.style.selectable.normal = nk_style_item_color(nk_rgb(206,206,206));\
	ctx.style.selectable.hover = nk_style_item_color(nk_rgb(206,206,206));\
	ctx.style.selectable.pressed = nk_style_item_color(nk_rgb(206,206,206));\
	ctx.style.selectable.normal_active = nk_style_item_color(nk_rgb(185,205,248));\
	ctx.style.selectable.hover_active = nk_style_item_color(nk_rgb(185,205,248));\
	ctx.style.selectable.pressed_active = nk_style_item_color(nk_rgb(185,205,248));\
	ctx.style.selectable.text_normal = nk_rgb(95,95,95);\
	ctx.style.selectable.text_hover = nk_rgb(95,95,95);\
	ctx.style.selectable.text_pressed = nk_rgb(95,95,95);\
	ctx.style.selectable.text_normal_active = nk_rgb(95,95,95);\
	ctx.style.selectable.text_hover_active = nk_rgb(95,95,95);\
	ctx.style.selectable.text_pressed_active = nk_rgb(95,95,95);\
}

#define NK_STYLE_SLIDER(ctx){\
	/* slider */\
	ctx.style.slider.normal          = nk_style_item_hide();\
	ctx.style.slider.hover           = nk_style_item_hide();\
	ctx.style.slider.active          = nk_style_item_hide();\
	ctx.style.slider.bar_normal      = nk_rgb(156,156,156);\
	ctx.style.slider.bar_hover       = nk_rgb(156,156,156);\
	ctx.style.slider.bar_active      = nk_rgb(156,156,156);\
	ctx.style.slider.bar_filled      = nk_rgb(156,156,156);\
	ctx.style.slider.cursor_normal.data.color = nk_rgb(156, 156, 156);\
	ctx.style.slider.cursor_hover.data.color  = nk_rgb(156, 156, 156);\
	ctx.style.slider.cursor_active.data.color = nk_rgb(156, 156, 156);\
	ctx.style.slider.cursor_size     = nk_vec2(16.5f,21);\
	ctx.style.slider.bar_height      = 1;\
}

#define NK_STYLE_PROGRESSBAR(ctx){\
	/* progressbar */\
	ctx.style.progress.normal = nk_style_item_color(nk_rgb(231,231,231));\
	ctx.style.progress.hover = nk_style_item_color(nk_rgb(231,231,231));\
	ctx.style.progress.active = nk_style_item_color(nk_rgb(231,231,231));\
	ctx.style.progress.cursor_normal = nk_style_item_color(nk_rgb(63,242,93));\
	ctx.style.progress.cursor_hover = nk_style_item_color(nk_rgb(63,242,93));\
	ctx.style.progress.cursor_active = nk_style_item_color(nk_rgb(63,242,93));\
	ctx.style.progress.border_color = nk_rgb(114,116,115);\
	ctx.style.progress.padding = nk_vec2(0,0);\
	ctx.style.progress.border = 2;\
	ctx.style.progress.rounding = 1;\
}

#define NK_STYLE_COMBO(ctx){\
	/* combo */\
	ctx.style.combo.normal = nk_style_item_color(nk_rgb(216,216,216));\
	ctx.style.combo.hover = nk_style_item_color(nk_rgb(216,216,216));\
	ctx.style.combo.active = nk_style_item_color(nk_rgb(216,216,216));\
	ctx.style.combo.border_color = nk_rgb(95,95,95);\
	ctx.style.combo.label_normal = nk_rgb(95,95,95);\
	ctx.style.combo.label_hover = nk_rgb(95,95,95);\
	ctx.style.combo.label_active = nk_rgb(95,95,95);\
	ctx.style.combo.border = 1;\
	ctx.style.combo.rounding = 1;\
\
	/* combo button */\
	ctx.style.combo.button.normal = nk_style_item_color(nk_rgb(216,216,216));\
	ctx.style.combo.button.hover = nk_style_item_color(nk_rgb(216,216,216));\
	ctx.style.combo.button.active = nk_style_item_color(nk_rgb(216,216,216));\
	ctx.style.combo.button.text_background = nk_rgb(216,216,216);\
	ctx.style.combo.button.text_normal = nk_rgb(95,95,95);\
	ctx.style.combo.button.text_hover = nk_rgb(95,95,95);\
	ctx.style.combo.button.text_active = nk_rgb(95,95,95);\
}

#define NK_STYLE_PROPERTY(ctx){\
	/* property */\
	ctx.style.property.normal = nk_style_item_color(nk_rgb(216,216,216));\
	ctx.style.property.hover = nk_style_item_color(nk_rgb(216,216,216));\
	ctx.style.property.active = nk_style_item_color(nk_rgb(216,216,216));\
	ctx.style.property.border_color = nk_rgb(81,81,81);\
	ctx.style.property.label_normal = nk_rgb(95,95,95);\
	ctx.style.property.label_hover = nk_rgb(95,95,95);\
	ctx.style.property.label_active = nk_rgb(95,95,95);\
	ctx.style.property.sym_left = NK_SYMBOL_TRIANGLE_LEFT;\
	ctx.style.property.sym_right = NK_SYMBOL_TRIANGLE_RIGHT;\
	ctx.style.property.rounding = 10;\
	ctx.style.property.border = 1;\
	\
	/* edit */\
	ctx.style.edit.normal = nk_style_item_color(nk_rgb(240,240,240));\
	ctx.style.edit.hover = nk_style_item_color(nk_rgb(240,240,240));\
	ctx.style.edit.active = nk_style_item_color(nk_rgb(240,240,240));\
	ctx.style.edit.border_color = nk_rgb(62,62,62);\
	ctx.style.edit.cursor_normal = nk_rgb(99,202,255);\
	ctx.style.edit.cursor_hover = nk_rgb(99,202,255);\
	ctx.style.edit.cursor_text_normal = nk_rgb(95,95,95);\
	ctx.style.edit.cursor_text_hover = nk_rgb(95,95,95);\
	ctx.style.edit.text_normal = nk_rgb(95,95,95);\
	ctx.style.edit.text_hover = nk_rgb(95,95,95);\
	ctx.style.edit.text_active = nk_rgb(95,95,95);\
	ctx.style.edit.selected_normal = nk_rgb(99,202,255);\
	ctx.style.edit.selected_hover = nk_rgb(99,202,255);\
	ctx.style.edit.selected_text_normal = nk_rgb(95,95,95);\
	ctx.style.edit.selected_text_hover = nk_rgb(95,95,95);\
	ctx.style.edit.border = 1;\
	ctx.style.edit.rounding = 2;\
	\
	/* property buttons */\
	ctx.style.property.dec_button.normal = nk_style_item_color(nk_rgb(216,216,216));\
	ctx.style.property.dec_button.hover = nk_style_item_color(nk_rgb(216,216,216));\
	ctx.style.property.dec_button.active = nk_style_item_color(nk_rgb(216,216,216));\
	ctx.style.property.dec_button.text_background = nk_rgba(0,0,0,0);\
	ctx.style.property.dec_button.text_normal = nk_rgb(95,95,95);\
	ctx.style.property.dec_button.text_hover = nk_rgb(95,95,95);\
	ctx.style.property.dec_button.text_active = nk_rgb(95,95,95);\
	ctx.style.property.inc_button = ctx.style.property.dec_button;\
	\
	/* property edit */\
	ctx.style.property.edit.normal = nk_style_item_color(nk_rgb(216,216,216));\
	ctx.style.property.edit.hover = nk_style_item_color(nk_rgb(216,216,216));\
	ctx.style.property.edit.active = nk_style_item_color(nk_rgb(216,216,216));\
	ctx.style.property.edit.border_color = nk_rgba(0,0,0,0);\
	ctx.style.property.edit.cursor_normal = nk_rgb(95,95,95);\
	ctx.style.property.edit.cursor_hover = nk_rgb(95,95,95);\
	ctx.style.property.edit.cursor_text_normal = nk_rgb(216,216,216);\
	ctx.style.property.edit.cursor_text_hover = nk_rgb(216,216,216);\
	ctx.style.property.edit.text_normal = nk_rgb(95,95,95);\
	ctx.style.property.edit.text_hover = nk_rgb(95,95,95);\
	ctx.style.property.edit.text_active = nk_rgb(95,95,95);\
	ctx.style.property.edit.selected_normal = nk_rgb(95,95,95);\
	ctx.style.property.edit.selected_hover = nk_rgb(95,95,95);\
	ctx.style.property.edit.selected_text_normal = nk_rgb(216,216,216);\
	ctx.style.property.edit.selected_text_hover = nk_rgb(216,216,216);\
}

#define NK_STYLE_CHART(ctx){\
	/* chart */\
	ctx.style.chart.background = nk_style_item_color(nk_rgb(216,216,216));\
	ctx.style.chart.border_color = nk_rgb(81,81,81);\
	ctx.style.chart.color = nk_rgb(95,95,95);\
	ctx.style.chart.selected_color = nk_rgb(255,0,0);\
	ctx.style.chart.border = 1;\
}

#endif
