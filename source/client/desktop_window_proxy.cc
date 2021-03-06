//
// Aspia Project
// Copyright (C) 2020 Dmitry Chapyshev <dmitry@aspia.ru>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.
//

#include "client/desktop_window_proxy.h"

#include "base/logging.h"
#include "base/task_runner.h"
#include "base/version.h"
#include "client/desktop_control_proxy.h"
#include "client/desktop_window.h"
#include "client/frame_factory.h"
#include "proto/desktop.pb.h"
#include "proto/desktop_extensions.pb.h"

namespace client {

DesktopWindowProxy::DesktopWindowProxy(std::shared_ptr<base::TaskRunner> ui_task_runner,
                                       DesktopWindow* desktop_window)
    : ui_task_runner_(std::move(ui_task_runner)),
      desktop_window_(desktop_window)
{
    frame_factory_ = desktop_window_->frameFactory();
}

DesktopWindowProxy::~DesktopWindowProxy()
{
    DCHECK(!desktop_window_);
}

void DesktopWindowProxy::dettach()
{
    DCHECK(ui_task_runner_->belongsToCurrentThread());
    desktop_window_ = nullptr;
}

void DesktopWindowProxy::configRequired()
{
    if (!ui_task_runner_->belongsToCurrentThread())
    {
        ui_task_runner_->postTask(
            std::bind(&DesktopWindowProxy::configRequired, shared_from_this()));
        return;
    }

    if (desktop_window_)
        desktop_window_->configRequired();
}

void DesktopWindowProxy::setCapabilities(
    const std::string& extensions, uint32_t video_encodings)
{
    if (!ui_task_runner_->belongsToCurrentThread())
    {
        ui_task_runner_->postTask(std::bind(&DesktopWindowProxy::setCapabilities,
                                            shared_from_this(),
                                            extensions,
                                            video_encodings));
        return;
    }

    if (desktop_window_)
        desktop_window_->setCapabilities(extensions, video_encodings);
}

void DesktopWindowProxy::setScreenList(const proto::ScreenList& screen_list)
{
    if (!ui_task_runner_->belongsToCurrentThread())
    {
        ui_task_runner_->postTask(
            std::bind(&DesktopWindowProxy::setScreenList, shared_from_this(), screen_list));
        return;
    }

    if (desktop_window_)
        desktop_window_->setScreenList(screen_list);
}

void DesktopWindowProxy::setSystemInfo(const proto::SystemInfo& system_info)
{
    if (!ui_task_runner_->belongsToCurrentThread())
    {
        ui_task_runner_->postTask(
            std::bind(&DesktopWindowProxy::setSystemInfo, shared_from_this(), system_info));
        return;
    }

    if (desktop_window_)
        desktop_window_->setSystemInfo(system_info);
}

std::shared_ptr<desktop::Frame> DesktopWindowProxy::allocateFrame(const desktop::Size& size)
{
    return frame_factory_->allocateFrame(size);
}

void DesktopWindowProxy::showWindow(
    std::shared_ptr<DesktopControlProxy> desktop_control_proxy, const base::Version& peer_version)
{
    if (!ui_task_runner_->belongsToCurrentThread())
    {
        ui_task_runner_->postTask(std::bind(&DesktopWindowProxy::showWindow,
                                            shared_from_this(),
                                            desktop_control_proxy,
                                            peer_version));
        return;
    }

    if (desktop_window_)
        desktop_window_->showWindow(desktop_control_proxy, peer_version);
}

void DesktopWindowProxy::drawFrame(std::shared_ptr<desktop::Frame> frame)
{
    if (!ui_task_runner_->belongsToCurrentThread())
    {
        ui_task_runner_->postTask(
            std::bind(&DesktopWindowProxy::drawFrame, shared_from_this(), frame));
        return;
    }

    if (desktop_window_)
        desktop_window_->drawFrame(frame);
}

void DesktopWindowProxy::drawMouseCursor(std::shared_ptr<desktop::MouseCursor> mouse_cursor)
{
    if (!ui_task_runner_->belongsToCurrentThread())
    {
        ui_task_runner_->postTask(
            std::bind(&DesktopWindowProxy::drawMouseCursor, shared_from_this(), mouse_cursor));
        return;
    }

    if (desktop_window_)
        desktop_window_->drawMouseCursor(mouse_cursor);
}

void DesktopWindowProxy::injectClipboardEvent(const proto::ClipboardEvent& event)
{
    if (!ui_task_runner_->belongsToCurrentThread())
    {
        ui_task_runner_->postTask(
            std::bind(&DesktopWindowProxy::injectClipboardEvent, shared_from_this(), event));
        return;
    }

    if (desktop_window_)
        desktop_window_->injectClipboardEvent(event);
}

} // namespace client
