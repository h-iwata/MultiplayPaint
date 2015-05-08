
#pragma once
class BaseView {
public:
	virtual ~BaseView() {}
	// 
	virtual void info(const char* format, ...);
	virtual void warn(const char* format, ...);
	virtual void error(const char* format, ...);

	virtual void onStateChange(int state, const char* stateStr, bool inLobby, bool inRoom);
	virtual void refreshStateInfo(const char* stateInfo);
	virtual void refreshMessage(const char* msg);

    virtual void setup();
    virtual void setupBoard(int tileCols, int tileRows);
	virtual void updateBoard(unsigned char* tiles, unsigned char flippedTiles[2]);
	virtual void updateRoomList(const char* roomNames[], unsigned int size);
	virtual void updateSavedRoomList(const char* savedRoomNames[], unsigned int size);
};
