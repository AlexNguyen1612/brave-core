/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/ui/views/omnibox/brave_search_conversion_promotion_view.h"

#include <memory>

#include "base/logging.h"
#include "brave/grit/brave_generated_resources.h"
#include "brave/grit/brave_theme_resources.h"
#include "cc/paint/paint_flags.h"
#include "third_party/skia/include/core/SkPath.h"
#include "ui/base/metadata/metadata_impl_macros.h"
#include "ui/base/models/image_model.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/geometry/insets.h"
#include "ui/views/cascading_property.h"
#include "ui/views/controls/image_view.h"
#include "ui/views/layout/flex_layout.h"

using brave_search_conversion::ConversionType;

////////////////////////////////////////////////////////////////////////////////
// OmniboxResultSelectionIndicator

class BraveOmniboxResultSelectionIndicator : public views::View {
 public:
  METADATA_HEADER(BraveOmniboxResultSelectionIndicator);

  static constexpr int kStrokeThickness = 3;

  explicit BraveOmniboxResultSelectionIndicator(
      BraveSearchConversionPromotionView* parent_view)
      : parent_view_(parent_view) {
    SetPreferredSize({kStrokeThickness, 0});
  }

  // views::View:
  void OnPaint(gfx::Canvas* canvas) override {
    SkPath path = GetPath();
    cc::PaintFlags flags;
    flags.setAntiAlias(true);
    flags.setColor(color_);
    flags.setStyle(cc::PaintFlags::kFill_Style);
    canvas->DrawPath(path, flags);
  }

  // views::View:
  void OnThemeChanged() override {
    views::View::OnThemeChanged();

    color_ = views::GetCascadingAccentColor(parent_view_);
  }

 private:
  SkColor color_;

  // Pointer to the parent view.
  const raw_ptr<BraveSearchConversionPromotionView> parent_view_;

  // The focus bar is a straight vertical line with half-rounded endcaps. Since
  // this geometry is nontrivial to represent using primitives, it's instead
  // represented using a fill path. This matches the style and implementation
  // used in Tab Groups.
  SkPath GetPath() const {
    SkPath path;

    path.moveTo(0, 0);
    path.arcTo(kStrokeThickness, kStrokeThickness, 0, SkPath::kSmall_ArcSize,
               SkPathDirection::kCW, kStrokeThickness, kStrokeThickness);
    path.lineTo(kStrokeThickness, height() - kStrokeThickness);
    path.arcTo(kStrokeThickness, kStrokeThickness, 0, SkPath::kSmall_ArcSize,
               SkPathDirection::kCW, 0, height());
    path.close();

    return path;
  }
};

BEGIN_METADATA(BraveOmniboxResultSelectionIndicator, views::View)
END_METADATA

BraveSearchConversionPromotionView::BraveSearchConversionPromotionView() {
  SetLayoutManager(std::make_unique<views::FlexLayout>())
      ->SetOrientation(views::LayoutOrientation::kHorizontal);
}

BraveSearchConversionPromotionView::~BraveSearchConversionPromotionView() =
    default;

void BraveSearchConversionPromotionView::SetType(
    brave_search_conversion::ConversionType type) {
  type_ = type;

  // Do nothing as this view is hidden for |kNone|.
  if (type_ == ConversionType::kNone)
    return;

  if (type_ == ConversionType::kButton) {
    if (!selection_indicator_) {
      selection_indicator_ = AddChildView(
          std::make_unique<BraveOmniboxResultSelectionIndicator>(this));
    }

    if (!icon_view_) {
      auto& bundle = ui::ResourceBundle::GetSharedInstance();
      icon_view_ = AddChildView(std::make_unique<views::ImageView>(
          ui::ImageModel::FromImageSkia(*bundle.GetImageSkiaNamed(
              IDR_BRAVE_SEARCH_LOGO_IN_SEARCH_PROMOTION))));
      icon_view_->SetProperty(views::kMarginsKey,
                              gfx::Insets::TLBR(14, 12, 36, 8));
    }
    selection_indicator_->SetVisible(true);
    icon_view_->SetVisible(true);
  } else {
    if (selection_indicator_)
      selection_indicator_->SetVisible(true);
    if (icon_view_)
      icon_view_->SetVisible(false);
  }
}

gfx::Size BraveSearchConversionPromotionView::CalculatePreferredSize() const {
  const int height = type_ == ConversionType::kButton ? 66 : 116;
  return gfx::Size(0, height);
}

BEGIN_METADATA(BraveSearchConversionPromotionView, views::View)
END_METADATA
