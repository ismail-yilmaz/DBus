#ifndef _DBusLib_MediaPlayer_h_
#define _DBusLib_MediaPlayer_h_

#include <DBus/DBus.h>

namespace Upp {

class DBusMediaPlayer {
public:
    enum class Status {
        Stopped,
        Playing,
        Paused,
        Unknown
    };

	struct MetaData : Moveable<MetaData> {
	    String         trackid;
	    int64          length;
	    String         arturl;
	    String         album;
	    String         astext;
	    int            audiobpm;
	    double         autorating;
	    String         contentcreated;
	    int            discnumber;
	    String         firstused;
	    String         lastused;
	    String         title;
	    int            tracknumber;
	    String         url;
	    int            usecount;
	    double         userrating;
	    Vector<String> albumartist;
	    Vector<String> artist;
	    Vector<String> comment;
	    Vector<String> composer;
	    Vector<String> genre;
	    Vector<String> lyricist;
	};
	    
    DBusMediaPlayer(DBusConnection& session);
    virtual ~DBusMediaPlayer() {}
    
    DBusMediaPlayer& Target(const String& player);
    String           GetTarget() const                { return target; }
    
    bool             FetchPlayers();
    Vector<String>   GetPlayers() const;
    
    bool             FetchStatus();
    Status           GetStatus() const;
    
    bool             FetchMetaData();
    MetaData         GetMetaData() const;
    DBusValueMap     GetRawMetaData() const           { return GetFirstArg<DBusValueMap>(dbus.GetMessage(), Null); }

    bool             FetchPosition();
    int64            GetPosition() const              { return GetFirstArg<int64>(dbus.GetMessage(), -1); }

    bool             FetchVolume();
    double           GetVolume() const                { return GetFirstArg<double>(dbus.GetMessage(), -1.0); }
    bool             SetVolume(double vol);

    bool             FetchRate();
    double           GetRate() const                  { return GetFirstArg<double>(dbus.GetMessage(), 1.0); }
    
    bool             Raise();
    bool             Quit();
              
    bool             Play();
    bool             Pause();
    bool             PlayPause();
    bool             Stop();
    bool             Prev();
    bool             Next();
    
    bool             Seek(int64 offset);
    bool             SetPosition(const String& tid, int64 pos);
    bool             OpenUri(const String& uri);
    
private:
    DBusConnection&  dbus;
    String           target;
};

inline constexpr const char* StdDBusMprisPath      = "/org/mpris/MediaPlayer2";
inline constexpr const char* StdDBusMprisRoot      = "org.mpris.MediaPlayer2";
inline constexpr const char* StdDBusMprisPlayer    = "org.mpris.MediaPlayer2.Player";
inline constexpr const char* StdDBusMprisTrackList = "org.mpris.MediaPlayer2.TrackList";
inline constexpr const char* StdDBusMprisPlaylists = "org.mpris.MediaPlayer2.Playlists";
inline constexpr const char* StdDBusMprisPrefix    = "org.mpris.MediaPlayer2.";

}
#endif