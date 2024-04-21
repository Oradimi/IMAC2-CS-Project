#pragma once

#include "implot/implot.h"
#include "math.hpp"
#include <iostream>

enum MathSwitchTabs { BETA, IRWINHALL, LAPLACE };

class MathTests {
private:
  MathSwitchTabs switchTabs = BETA;
  RandomMath rand;

  double testBeta1[10000];
  double testBeta2[10000];
  double testIrwinHall1[10000];
  double testIrwinHall2[10000];
  double testLaplace1[10000];
  double testLaplace2[10000];

public:
  MathTests() = default;

  void initiate() {
    for (int i = 0; i < 100; i++) {
      std::cout << rand.generateUniformDiscrete(0, 5) << " ";
    }

    for (double &i : testBeta1) {
      i = rand.generateBeta(2.0, 5.0);
    }

    for (double &i : testBeta2) {
      i = rand.generateBeta(2.0, 2.0);
    }

    for (double &i : testIrwinHall1) {
      i = rand.generateIrwinHall();
    }

    for (double &i : testIrwinHall2) {
      i = rand.generateIrwinHall(2, 0.0);
    }

    for (double &i : testLaplace1) {
      i = rand.generateLaplace(0.0, 5.0);
    }

    for (double &i : testLaplace2) {
      i = rand.generateLaplace(-10.0, 2.0);
    }
  }

  void displayTestsGUI() {
    if (ImGui::Button("Beta", ImVec2(100.0f, 0.0f)))
      switchTabs = BETA;
    ImGui::SameLine(0.0, 2.0f);
    if (ImGui::Button("Irwin-Hall", ImVec2(100.0f, 0.0f)))
      switchTabs = IRWINHALL;
    ImGui::SameLine(0.0, 2.0f);
    if (ImGui::Button("Laplace", ImVec2(100.0f, 0.0f)))
      switchTabs = LAPLACE;

    switch (switchTabs) {
    case BETA:
      if (ImPlot::BeginPlot("Beta PDF")) {
        ImPlot::PlotHistogram("alpha 2, beta 5", testBeta1, 10000, 50, 1.0,
                              ImPlotRange());
        ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
        ImPlot::SetNextMarkerStyle(ImPlotMarker_Square);
        ImPlot::PlotHistogram("alpha 2, beta 2", testBeta2, 10000, 50, 1.0,
                              ImPlotRange());
        ImPlot::PopStyleVar();
        ImPlot::EndPlot();
      }
      break;
    case IRWINHALL:
      if (ImPlot::BeginPlot("Irwin-Hall PDF")) {
        ImPlot::PlotHistogram("n 12, mu -6", testIrwinHall1, 10000, 50, 1.0,
                              ImPlotRange());
        ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
        ImPlot::SetNextMarkerStyle(ImPlotMarker_Square);
        ImPlot::PlotHistogram("n 2, mu 0", testIrwinHall2, 10000, 50, 1.0,
                              ImPlotRange());
        ImPlot::PopStyleVar();
        ImPlot::EndPlot();
      }
      break;
    case LAPLACE:
      if (ImPlot::BeginPlot("Laplace PDF")) {
        ImPlot::PlotHistogram("mu 0, b 5", testLaplace1, 10000, 50, 1.0,
                              ImPlotRange());
        ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
        ImPlot::SetNextMarkerStyle(ImPlotMarker_Square);
        ImPlot::PlotHistogram("mu -10, b 2", testLaplace2, 10000, 50, 1.0,
                              ImPlotRange());
        ImPlot::PopStyleVar();
        ImPlot::EndPlot();
      }
      break;
    }
  }
};
