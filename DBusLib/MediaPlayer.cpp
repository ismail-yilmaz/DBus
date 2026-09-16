#include "MediaPlayer.h"

namespace Upp {

namespace {

void MapToMetaData(const DBusValueMap& map, DBusMediaPlayer::MetaData& mdata)
{
	if(map.IsEmpty())
		return;
	
	auto Fetch = [&](const char* key, auto& field) {
		using T = std::remove_reference_t<decltype(field)>;
		if(const DBusValue* val = map.FindPtr(key); val) {
			if constexpr (std::is_same_v<T, String>) {
				if(val->Is<String>())
					field = val->To<String>();
			}
			else
			if constexpr (std::is_arithmetic_v<T>) {
				// If the player sent the wrong numeric type, catch it and cast safely.
				// E.g. spotify client does this by sending int32 instead of int64...
				if(val->Is<byte>())
					field = (T) val->To<byte>();
				else
				if(val->Is<int16>())
					field = (T) val->To<int16>();
				else
				if(val->Is<uint16>())
					field = (T) val->To<uint16>();
				else
				if(val->Is<int32>())
					field = (T) val->To<int32>();
				else
				if(val->Is<uint32>())
					field = (T) val->To<uint32>();
				else
				if(val->Is<int64>())
					field = (T) val->To<int64>();
				else
				if(val->Is<uint64>())
					field = (T) val->To<uint64>();
				else
				if(val->Is<double>())
					field = (T) val->To<double>();
			}
		}
	};

	// Handles players that send a String instead of an Array
	auto FetchVec = [&](const char* key, Vector<String>& vec) {
		if(const DBusValue* val = map.FindPtr(key); val) {
			if(val->Is<DBusValueArray>()) {
				if(DBusValueArray va = val->To<DBusValueArray>(); va.GetCount() > 0) {
					vec.Reserve(va.GetCount());
					for(int i = 0; i < va.GetCount(); i++) {
						if(va[i].Is<String>())
							vec.Add(va[i].To<String>());
					}
				}
			}
			// Spec Violation Fallback: The player sent a single string
			else
			if(val->Is<String>())
				vec.Add(val->To<String>());
		}
	};

	Fetch("mpris:trackid",        mdata.trackid);
	Fetch("mpris:length",         mdata.length);
	Fetch("mpris:artUrl",         mdata.arturl);
	Fetch("xesam:album",          mdata.album);
	Fetch("xesam:asText",         mdata.astext);
	Fetch("xesam:audioBPM",       mdata.audiobpm);
	Fetch("xesam:autoRating",     mdata.autorating);
	Fetch("xesam:contentCreated", mdata.contentcreated);
	Fetch("xesam:discNumber",     mdata.discnumber);
	Fetch("xesam:firstUsed",      mdata.firstused);
	Fetch("xesam:lastUsed",       mdata.lastused);
	Fetch("xesam:title",          mdata.title);
	Fetch("xesam:trackNumber",    mdata.tracknumber);
	Fetch("xesam:url",            mdata.url);
	Fetch("xesam:useCount",       mdata.usecount);
	Fetch("xesam:userRating",     mdata.userrating);
	FetchVec("xesam:lyricist",    mdata.lyricist);
	FetchVec("xesam:comment",     mdata.comment);
	FetchVec("xesam:composer",    mdata.composer);
	FetchVec("xesam:genre",       mdata.genre);
	FetchVec("xesam:albumArtist", mdata.albumartist);
	FetchVec("xesam:artist",      mdata.artist);
}

}

DBusMediaPlayer::DBusMediaPlayer(DBusConnection& session)
: dbus(session)
{
}

DBusMediaPlayer& DBusMediaPlayer::Target(const String& player)
{
	target = player.StartsWith(StdDBusMprisPrefix) ? player : StdDBusMprisPrefix + player;
	return *this;
}

bool DBusMediaPlayer::FetchPlayers()
{
	return dbus.BusMethodCall("ListNames");
}

Vector<String> DBusMediaPlayer::GetPlayers() const
{
	Vector<String> players;
	for(const String& s : dbus.GetMessage().ParseStringArray())
		if(s.StartsWith(StdDBusMprisPrefix))
			players << s;
	return players;
}

bool DBusMediaPlayer::FetchStatus()
{
	return dbus.MethodCall(
		target,
		StdDBusMprisPath,
		StdDBusProperties,
		"Get", { StdDBusMprisPlayer, "PlaybackStatus" }
	);
}

DBusMediaPlayer::Status DBusMediaPlayer::GetStatus() const
{
	return decode(
		GetFirstArg<String>(dbus.GetMessage(), Null),
		"Playing", Status::Playing,
		"Paused",  Status::Paused,
		"Stopped", Status::Stopped,
		 /* Unknonw */ Status::Unknown
	);
}

bool DBusMediaPlayer::FetchMetaData()
{
	return dbus.MethodCall(
		target,
		StdDBusMprisPath,
		StdDBusProperties,
		"Get", { StdDBusMprisPlayer, "Metadata" }
	);
}

DBusMediaPlayer::MetaData DBusMediaPlayer::GetMetaData() const
{
	MetaData mdata;
	MapToMetaData(GetRawMetaData(), mdata);
	return pick(mdata);
}

bool DBusMediaPlayer::FetchPosition()
{
	return dbus.MethodCall(
		target,
		StdDBusMprisPath,
		StdDBusProperties,
		"Get", { StdDBusMprisPlayer, "Position" }
	);
}

bool DBusMediaPlayer::FetchVolume()
{
	return dbus.MethodCall(
		target,
		StdDBusMprisPath,
		StdDBusProperties,
		"Get", { StdDBusMprisPlayer, "Volume" }
	);
}

bool DBusMediaPlayer::SetVolume(double vol)
{
	return dbus.MethodCall(
		target,
		StdDBusMprisPath,
		StdDBusProperties,
		"Set", { StdDBusMprisPlayer, "Volume", AsVariant(vol) }
	);
}

bool DBusMediaPlayer::FetchRate()
{
	return dbus.MethodCall(
		target,
		StdDBusMprisPath,
		StdDBusProperties,
		"Get", { StdDBusMprisPlayer, "Rate" }
	);
}

bool DBusMediaPlayer::Raise()
{
	return dbus.MethodCall(
		target,
		StdDBusMprisPath,
		StdDBusMprisRoot,
		"Raise"
	);
}

bool DBusMediaPlayer::Quit()
{
	return dbus.MethodCall(
		target,
		StdDBusMprisPath,
		StdDBusMprisRoot,
		"Quit"
	);
}

bool DBusMediaPlayer::Play()
{
	return dbus.MethodCall(
		target,
		StdDBusMprisPath,
		StdDBusMprisPlayer,
		"Play"
	);
}

bool DBusMediaPlayer::Pause()
{
	return dbus.MethodCall(
		target,
		StdDBusMprisPath,
		StdDBusMprisPlayer,
		"Pause"
	);
}

bool DBusMediaPlayer::PlayPause()
{
	return dbus.MethodCall(
		target,
		StdDBusMprisPath,
		StdDBusMprisPlayer,
		"PlayPause"
	);
}

bool DBusMediaPlayer::Stop()
{
	return dbus.MethodCall(
		target,
		StdDBusMprisPath,
		StdDBusMprisPlayer,
		"Stop"
	);
}

bool DBusMediaPlayer::Prev()
{
	return dbus.MethodCall(
		target,
		StdDBusMprisPath,
		StdDBusMprisPlayer,
		"Previous"
	);
}

bool DBusMediaPlayer::Next()
{
	return dbus.MethodCall(
		target,
		StdDBusMprisPath,
		StdDBusMprisPlayer,
		"Next"
	);
}

bool DBusMediaPlayer::Seek(int64 offset)
{
	return dbus.MethodCall(
		target,
		StdDBusMprisPath,
		StdDBusMprisPlayer,
		"Seek", { offset }
	);
}

bool DBusMediaPlayer::SetPosition(const String& tid, int64 pos)
{
	return dbus.MethodCall(
		target,
		StdDBusMprisPath,
		StdDBusMprisPlayer,
		"SetPosition", { tid, pos }
	);
}

bool DBusMediaPlayer::OpenUri(const String& uri)
{
	return dbus.MethodCall(
		target,
		StdDBusMprisPath,
		StdDBusMprisRoot,
		"OpenUri", { uri }
	);
}

}