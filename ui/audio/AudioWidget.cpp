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

#include <Wt/WBreak>

#include "logger/Logger.hpp"

#include "AudioWidget.hpp"

namespace UserInterface {

AudioWidget::AudioWidget(SessionData& sessionData, Wt::WContainerWidget* parent)
: Wt::WContainerWidget(parent),
_db(sessionData.getDatabaseHandler()),
_audioDbWidget(nullptr),
_mediaPlayer(nullptr),
_imgResource(nullptr),
_img(nullptr)
{
	_audioDbWidget = new AudioDatabaseWidget(sessionData.getDatabaseHandler(), this);

	_audioDbWidget->trackSelected().connect(this, &AudioWidget::playTrack);

	_mediaPlayer = new AudioMediaPlayerWidget(this);
	_mediaPlayer->playbackEnded().connect(this, &AudioWidget::handleTrackEnded);
	this->addWidget(new Wt::WBreak());

	// Image
	_imgResource = new Wt::WMemoryResource(this);
	_imgLink.setResource( _imgResource);
	_img = new Wt::WImage(_imgLink, this);

}

void
AudioWidget::search(const std::string& searchText)
{
	_audioDbWidget->search(searchText);
}


void
AudioWidget::playTrack(boost::filesystem::path p)
{
	LMS_LOG(MOD_UI, SEV_DEBUG) << "play track '" << p << "'";
	try {

		std::size_t bitrate = 0;

		// Get user preferences
		{
			Wt::Dbo::Transaction transaction(_db.getSession());
			Database::User::pointer user = _db.getCurrentUser();
			if (user)
				bitrate = user->getAudioBitrate();
			else
			{
				LMS_LOG(MOD_UI, SEV_ERROR) << "Can't play video: user does not exists!";
				return; // TODO logout?
			}
		}

		Transcode::InputMediaFile inputFile(p);

		Transcode::Parameters parameters(inputFile, Transcode::Format::get(Transcode::Format::OGA));

		parameters.setBitrate(Transcode::Stream::Audio, bitrate);

		_mediaPlayer->load( parameters );

		// Refresh cover
		{
			std::vector<CoverArt::CoverArt> covers = inputFile.getCovers();

			if (!covers.empty())
			{
				LMS_LOG(MOD_UI, SEV_DEBUG) << "Cover found!";
				if (!covers.front().scale(256)) // TODO
					LMS_LOG(MOD_UI, SEV_ERROR) << "Cannot resize!";

				//_imgResource->setMimeType(covers.front().getMimeType());
				_imgResource->setData(covers.front().getData());
			}
			else {
				LMS_LOG(MOD_UI, SEV_DEBUG) << "No cover found!";
				_imgResource->setData( std::vector<unsigned char>());
			}

			_imgResource->setChanged();
		}
	}
	catch( std::exception &e)
	{
		LMS_LOG(MOD_UI, SEV_ERROR) << "Caught exception while loading '" << p << "': " << e.what();
	}
}

	void
AudioWidget::handleTrackEnded(void)
{
	LMS_LOG(MOD_UI, SEV_DEBUG) << "Track playback ended!";
	_audioDbWidget->selectNextTrack();
}

} // namespace UserInterface

