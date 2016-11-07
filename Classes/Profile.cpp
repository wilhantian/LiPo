#include "Profile.h"


Profile* Profile::create(std::string filename)
{
	auto profile = new Profile();
	if (profile && profile->init(filename))
	{
		profile->autorelease();
		return profile;
	}
	delete profile;
	profile = nullptr;
	return nullptr;
}

bool Profile::init(std::string filename)
{
	this->filename = filename;
	map = FileUtils::getInstance()->getValueMapFromFile(filename);
	return true;
}

AnimationProfile* Profile::animationForKey(std::string animationName)
{
	if (animationMap[animationName] != nullptr)
	{
		return animationMap[animationName];
	}

	auto animations = map["animations"].asValueMap();
	auto anim = animations[animationName].asValueMap();
	auto delay = anim["delay"].asFloat();
	auto format = anim["format"].asString();
	auto frameCount = anim["frameCount"].asInt();

	std::vector<std::string> frameNames;
	for (int i = 0; i < frameCount; i++)
	{
		char buf[128];
		sprintf_s(buf, format.c_str(), i);
		frameNames.push_back(buf);
	}

	auto animationProfile = new AnimationProfile(frameNames, delay);
	animationMap[animationName] = animationProfile;
	return animationProfile;
}

Profile::Profile()
{

}

Profile::~Profile()
{
	std::for_each(animationMap.begin(), animationMap.end(), 
		[=](std::map<std::string, AnimationProfile*>::reference a
	){
		delete a.second;
	});
}

//////////////////////////////////////////////////
/// AnimationProfile
//////////////////////////////////////////////////
AnimationProfile::AnimationProfile(std::vector<std::string> filenames, float delay)
{
	this->delay = delay;
	this->frameSize = filenames.size();

	for (int i = 0; i < this->frameSize; i++)
	{
		auto texture = TextureCache::getInstance()->addImage(filenames[i]);
		frames.pushBack(texture);
	}
}

AnimationProfile::~AnimationProfile()
{

}

Vector<Texture2D*> AnimationProfile::getFrames()
{
	return this->frames;
}

float AnimationProfile::getDelay()
{
	return this->delay;
}

int AnimationProfile::getFrameSize()
{
	return this->frameSize;
}
