#include <atomic>     // for atomic
#include <cmath>      // for sin
#include <functional> // for ref, reference_wrapper, function
#include <stddef.h>   // for size_t
#include <string> // for string, basic_string, char_traits, operator+, to_string
#include <thread> // for sleep_for, thread
#include <vector> // for vector

#include "ftxui/component/component.hpp" // for Checkbox, Renderer, Horizontal, Vertical, Input, Menu, Radiobox, ResizableSplitLeft, Tab
#include "ftxui/component/component_base.hpp"    // for ComponentBase, Component
#include "ftxui/component/component_options.hpp" // for MenuOption, InputOption
#include "ftxui/component/event.hpp"             // for Event, Event::Custom
#include "ftxui/component/screen_interactive.hpp" // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp" // for text, color, operator|, bgcolor, filler, Element, vbox, size, hbox, separator, flex, window, graph, EQUAL, paragraph, WIDTH, hcenter, Elements, bold, vscroll_indicator, HEIGHT, flexbox, hflow, border, frame, flex_grow, gauge, paragraphAlignCenter, paragraphAlignJustify, paragraphAlignLeft, paragraphAlignRight, dim, spinner, LESS_THAN, center, yframe, GREATER_THAN
#include "ftxui/screen/color.hpp" // for Color, Color::BlueLight, Color::RedLight, Color::Black, Color::Blue, Color::Cyan, Color::CyanLight, Color::GrayDark, Color::GrayLight, Color::Green, Color::GreenLight, Color::Magenta, Color::MagentaLight, Color::Red, Color::White, Color::Yellow, Color::YellowLight, Color::Default, Color::Palette256, ftxui

using namespace ftxui;

std::string get_data(std::string title) {
  static uint32_t calls = 0;
  static uint32_t uptime = 0;
  calls++;
  if (title.compare("UTC") == 0) {
    return "11:59:59";
  } else if (title.compare("Uptime") == 0) {
    if ((calls % 20) == 0) {
      uptime++;
    }
    return std::to_string(uptime);
  } else if (title.compare("GPS Pos Type") == 0) {
    return "BAD";
  } else if (title.compare("INS State") == 0) {
    return "GOOD";
  } else if (title.compare("Current Draw") == 0) {
    return "0.000A";
  } else if (title.compare("Temperature") == 0) {
    return "22";
  } else if (title.compare("CPU Idle") == 0) {
    return "55.8";
  }

  return "";
}

Element data_display() {
  std::vector<std::string> fields = {
      "UTC",          "Uptime",      "GPS Pos Type", "INS State",
      "Current Draw", "Temperature", "CPU Idle",
  };
  Elements field_columns;
  Elements value_columns;
  uint32_t max_field_width = 5;
  uint32_t max_value_width = 5;
  field_columns.push_back(separator());
  value_columns.push_back(separator());
  uint32_t row = 0;
  for (auto &field : fields) {
    if ((row % 2) == 0) {
      field_columns.push_back(color(Color::LightSlateBlue, text(field)));
    } else {
      field_columns.push_back(color(Color::LightSkyBlue1, text(field)));
    }
    field_columns.push_back(separator());
    if (field.length() > max_field_width) {
      max_field_width = field.length();
    }
    std::string value = get_data(field);
    value_columns.push_back(color(Color::Default, text(value)));
    value_columns.push_back(separator());
    if (value.length() > max_value_width) {
      max_value_width = value.length();
    }
    row++;
  }

  return vbox({
      hbox({
          separator(),
          vbox(
              {field_columns | size(WIDTH, GREATER_THAN, max_field_width + 2)}),
          separator(),
          vbox(
              {value_columns | size(WIDTH, GREATER_THAN, max_value_width + 2)}),
          separator(),
      }),
  });
}

int main() {
  auto screen = ScreenInteractive::Fullscreen();

  // ---------------------------------------------------------------------------
  // Monitor
  // ---------------------------------------------------------------------------

  auto monitor_tab_renderer = Renderer([&] { return data_display(); });

  // ---------------------------------------------------------------------------
  // Tabs
  // ---------------------------------------------------------------------------

  int tab_index = 0;
  std::vector<std::string> tab_entries = {
      "monitor",
  };
  auto tab_selection =
      Menu(&tab_entries, &tab_index, MenuOption::HorizontalAnimated());
  auto tab_content = Container::Tab(
      {
          monitor_tab_renderer,
      },
      &tab_index);

  auto exit_button =
      Button("Exit", [&] { screen.Exit(); }, ButtonOption::Animated());

  auto main_container = Container::Vertical({
      Container::Horizontal({
          tab_selection,
          exit_button,
      }),
      tab_content,
  });

  auto main_renderer = Renderer(main_container, [&] {
    return vbox({
        text("FTXUI Demo") | bold | hcenter,
        hbox({
            tab_selection->Render() | flex,
            exit_button->Render(),
        }),
        tab_content->Render() | flex,
    });
  });

  std::atomic<bool> refresh_ui_continue = true;
  std::thread refresh_ui([&] {
    while (refresh_ui_continue) {
      using namespace std::chrono_literals;
      std::this_thread::sleep_for(0.05s);
      screen.Post([&] {});
      // After updating the state, request a new frame to be drawn. This is done
      // by simulating a new "custom" event to be handled.
      screen.Post(Event::Custom);
    }
  });

  screen.Loop(main_renderer);
  refresh_ui_continue = false;
  refresh_ui.join();

  return 0;
}
