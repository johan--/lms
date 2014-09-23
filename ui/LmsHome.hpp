/*
 * Copyright (C) 2013 Emeric Poupon
 *
 * This file is part of LMS.
 *
 * LMS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LMS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LMS.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LMS_HOME_HPP
#define LMS_HOME_HPP

#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>

#include "common/SessionData.hpp"

#include "audio/AudioWidget.hpp"
#include "video/VideoWidget.hpp"

namespace UserInterface {

class LmsHome : public Wt::WContainerWidget
{
	public:

		LmsHome(SessionData& sessionData, Wt::WContainerWidget* parent = 0);

	private:

		void handleSearch(void);
		void handleUserMenuSelected( Wt::WMenuItem* item );

		SessionData&	_sessionData;

		Wt::WLineEdit*	_searchEdit;
		AudioWidget*	_audioWidget;
		VideoWidget*	_videoWidget;
};

} // namespace UserInterface

#endif

