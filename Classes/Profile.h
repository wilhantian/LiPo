#ifndef _PROFILE_H_
#define _PROFILE_H_

#include "cocos2d.h"

USING_NS_CC;

class AnimationProfile;

class Profile : public Ref
{
public:
	static Profile* create(std::string filename);
	bool init(std::string filename);
	AnimationProfile* animationForKey(std::string animationName);

	Profile();
	~Profile();

public:
	std::string filename;
	ValueMap map;

	std::map<std::string, AnimationProfile*> animationMap;
};

class AnimationProfile
{
public:
	Vector<Texture2D*> getFrames();
	float getDelay();
	int getFrameSize();

	AnimationProfile(std::vector<std::string> filenames, float delay);
	~AnimationProfile();

private:
	float delay;
	Vector<Texture2D*> frames;
	int frameSize;
};

#endif