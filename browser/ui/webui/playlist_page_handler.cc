/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/ui/webui/playlist_page_handler.h"

#include <string>
#include <utility>
#include <vector>

#include "brave/browser/playlist/playlist_service_factory.h"
#include "brave/components/playlist/playlist_constants.h"
#include "chrome/browser/profiles/profile.h"

playlist::PlaylistService* GetPlaylistService(Profile* profile) {
  return playlist::PlaylistServiceFactory::GetForBrowserContext(profile);
}

PlaylistPageHandler::PlaylistPageHandler(
    Profile* profile,
    mojo::PendingReceiver<playlist::mojom::PageHandler> pending_page_handler,
    mojo::PendingRemote<playlist::mojom::Page> pending_page)
    : profile_(profile),
      page_(std::move(pending_page)),
      handler_(this, std::move(pending_page_handler)) {
  DCHECK(profile_);
  observation_.Observe(GetPlaylistService(profile_));
}

PlaylistPageHandler::~PlaylistPageHandler() = default;

void PlaylistPageHandler::GetAllPlaylists(
    PlaylistPageHandler::GetAllPlaylistsCallback callback) {
  std::vector<mojo::StructPtr<playlist::mojom::Playlist>> playlists;
  for (const auto& playlist : GetPlaylistService(profile_)->GetAllPlaylists()) {
    std::vector<mojo::StructPtr<playlist::mojom::PlaylistItem>> items;
    for (const auto& item : playlist.items) {
      items.push_back(playlist::mojom::PlaylistItem::New(
          item.id, item.title, GURL(item.media_file_path),
          GURL(item.thumbnail_path), item.ready));
    }
    playlists.push_back(playlist::mojom::Playlist::New(
        playlist.id, playlist.name, std::move(items)));
  }

  std::move(callback).Run(std::move(playlists));
}

void PlaylistPageHandler::GetPlaylist(
    const std::string& id,
    PlaylistPageHandler::GetPlaylistCallback callback) {
  const auto& playlist = GetPlaylistService(profile_)->GetPlaylist(id);
  if (!playlist.has_value()) {
    std::move(callback).Run(nullptr);
    return;
  }

  std::vector<mojo::StructPtr<playlist::mojom::PlaylistItem>> items;
  for (const auto& item : playlist->items) {
    items.push_back(playlist::mojom::PlaylistItem::New(
        item.id, item.title, GURL(item.media_file_path),
        GURL(item.thumbnail_path), item.ready));
  }
  std::move(callback).Run(playlist::mojom::Playlist::New(
      playlist->id, playlist->name, std::move(items)));
}

void PlaylistPageHandler::AddMediaFilesFromPageToPlaylist(const std::string& id,
                                                          const GURL& url) {
  GetPlaylistService(profile_)->RequestDownloadMediaFilesFromPage(id,
                                                                  url.spec());
}

void PlaylistPageHandler::RemoveItemFromPlaylist(const std::string& playlist_id,
                                                 const std::string& item_id) {
  GetPlaylistService(profile_)->RemoveItemFromPlaylist(playlist_id, item_id);
}

void PlaylistPageHandler::CreatePlaylist(
    playlist::mojom::PlaylistPtr playlist) {
  playlist::PlaylistInfo info;
  info.name = playlist->name;
  GetPlaylistService(profile_)->CreatePlaylist(info);
}

void PlaylistPageHandler::RemovePlaylist(const std::string& playlist_id) {
  GetPlaylistService(profile_)->RemovePlaylist(playlist_id);
}

void PlaylistPageHandler::OnPlaylistStatusChanged(
    const playlist::PlaylistChangeParams& params) {
  page_->OnEvent(playlist::mojom::PlaylistEvent::kItemAdded);
}
