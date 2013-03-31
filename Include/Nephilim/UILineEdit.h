#ifndef UILineEdit_h__
#define UILineEdit_h__

#include "Platform.h"
#include "UIControl.h"
#include "Text.h"

NEPHILIM_NS_BEGIN

class NEPHILIM_API UILineEdit : public UIControl{
public:
	UILineEdit();

	bool onEventNotification(Event& event);

	bool isFocusable(){return true;}
	virtual bool onKeyPressed(Keyboard::Key key);
	virtual bool onTextEvent(Uint32 code);

	void addCharacter(Uint32 charCode);
	void eraseCharacter();

	void setText(const String& text);

	void draw(Renderer* renderer);

	Text t;
	String s;

	int m_pipeIndex;
};

class ASEngine;
bool registerUILineEdit(ASEngine* engine);

NEPHILIM_NS_END
#endif // UILineEdit_h__