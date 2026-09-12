#include <DBusLib/DBusLib.h>

using namespace Upp;

String GetPlayerFromName(const String& s) { return StdDBusMprisPrefix + s; }

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	
	try {
		DBusConnection dbus;
		if(!dbus.Timeout(5000).ConnectSession())
			throw DBusError("Failed to connect to D-Bus session bus.");
	
		DBusMediaPlayer player(dbus);
		
		RLOG("Searching for MPRIS media players...");
		if(!player.FetchPlayers())
			throw DBusError("Failed to query D-Bus for MPRIS media players.");
		
		Vector<String> activeplayers = player.GetPlayers();
		if(activeplayers.IsEmpty())
			throw DBusError("No active MPRIS media players found.");
		
		RLOG("\nFound players:");
		for(const String& s : activeplayers)
			RLOG(" - " << s);
		
		// Try spotify first...
		int i = FindIndex(activeplayers, GetPlayerFromName("spotify"));
		String targetplayer = activeplayers[max(i, 0)];

		RLOG("\nTargeting: " << targetplayer);
		player.Target(targetplayer);
		
		// Check status
		if(player.FetchStatus()) {
			RLOG("\nQuerying player status...");
			DBusMediaPlayer::Status status = player.GetStatus();
			RLOG(" - Status: " << decode(status,
					DBusMediaPlayer::Status::Playing, "Playing",
					DBusMediaPlayer::Status::Paused,  "Paused",
					DBusMediaPlayer::Status::Stopped, "Stoped",
					"Unknown")
			);
		}
	
		// Fetch meta-data
		if(player.FetchMetaData()) {
			RLOG("\nRetrieving meta-data...");
			DBusMediaPlayer::MetaData data = player.GetMetaData();
			RLOG(" - Title:  " << data.title);
			RLOG(" - Artist: " << Join(data.artist, ","));
			RLOG(" - URL:    " << data.url);
		}
		
		// Get and set volume
		if(player.FetchVolume()) {
			RLOG("\nDynamic volume control...");
			RLOG(" - Current volume: " << player.GetVolume());
			RLOG(" - Setting volume to 0.5");
			player.SetVolume(0.5);
		}
		
		RLOG("\nIssuing playback commands...");
		
		RLOG(" -> Pausing...");
		player.Pause();
		Sleep(1000);
		
		RLOG(" -> Playing...");
		player.Play();
		Sleep(5000);

		RLOG(" -> Toggling...");
		player.PlayPause();
		
		RLOG("\nRaising MPRIS media player window...");
		player.Raise();
	
		RLOG("Done!");
	}
	catch(const DBusError& e) {
		RLOG(e);
	}
	catch(...) {
		RLOG("Unknown exception occured!");
	}
}
