#include <Core/Core.h>
#include <TextLib/TextLib.h>
using namespace Upp;

static constexpr int SCRIPT_COUNT = 5;
const char* scripts[SCRIPT_COUNT] = {
R"TXT(Scar tissue that I wish you saw
Sarcastic mister know-it-all
Close your eyes and Ill kiss you
Cause with the birds Ill share
With the birds, Ill share this lonely view
With the birds, Ill share this lonely view
Ah, push me up against the wall
Young Kentucky girl in a push-up bra
Ah, fallin all over myself
To lick your heart and taste your health, cause
With the birds , Ill share this lonely view
With the birds , Ill share this lonely view
With the birds , Ill share this lonely view
Blood loss in a bathroom stall
A southern girl with a scarlet drawl
I wave good-bye to Ma and Pa
Cause with the birds Ill share
With the birds , Ill share this lonely view
With the birds , Ill share this lonely view
Soft-spoken with a broken jaw
Step outside but not to brawl and
Autumns sweet, we call it fall
Ill make it to the moon if I have to crawl and
With the birds, Ill share this lonely view
With the birds, Ill share this lonely view
With the birds, Ill share this lonely view
Scar tissue that I wish you saw
Sarcastic mister know-it-all
Close your eyes and Ill kiss you
Cause with the birds Ill share
With the birds, Ill share this lonely view
With the birds, Ill share this lonely view
With the birds, Ill share this lonely view
)TXT",
R"TXT(Sometimes I feel like I dont have a partner
Sometimes I feel like my only friend
Is the city I live in, The City of Angels
Lonely as I am together we cry
I drive on her streets cause shes my companion
I walk through her hills cause she knows who I am
She sees my good deeds and she kisses me windy
Well, I never worry, now that is a lie
I dont ever wanna feel like I did that day
But take me to the place I love, take me all the way
I dont ever wanna feel like I did that day
But take me to the place I love take me all the way
Yeah, yeah yeah
Its hard to believe that theres nobody out there
Its hard to believe that Im all alone
At least I have her love
The city, she loves me
Lonely as I am together we cry
I dont ever wanna feel like I did that day
But take me to the place I love take me all the way
I dont ever wanna feel like I did that day
But take me to the place I love take me all the way
Yeah, yeah yeah
Oh, no, no, no, yeah yeah
Love me, I say, yeah yeah
One time
Is where I drew some blood
I could not get enough
Forgot about my love
I gave my life away, yeah
Yeah yeah
Oh, no, no no no, yeah yeah
Oh no, I said oh, yeah yeah
Here I stay)TXT",
R"TXT(Come to decide that the things that I tried
Were in my life just to get high on
When I sit alone, come get a little known
But I need more than myself this time
Step from the road to the sea to the sky
And I do believe that we rely on
When I lay it on, come get to play it on
All my life to sacrifice
Hey, oh
Listen what I say, oh
I got your— Hey, oh
Now listen what I say, oh
When will I know that I really cant go
To the well once more time to decide on
When its killing me, when will I really see
All that I need to look inside?
Come to believe that I better not leave
Before I get my chance to ride
When its killing me, what do I really need
All that I need to look inside?
Hey, oh
Listen what I say, oh
Come back and— Hey, oh
Look at what I say, oh
The more I see, the less I know
The more I like to let it go
Hey, oh! Whoa-whoa
Deep beneath the cover of another perfect wonder
Where its so white as snow
Finally divided by a world so undecided
And theres nowhere to go
In between the cover of another perfect wonder
And its so white as snow
Running through the field where all my tracks will be concealed
And theres nowhere to go
Ho!
When to descend to amend for a friend
All the channels that have broken down
Now you bring it up, Im gonna ring it up
Just to hear you sing it out
Step from the road to the sea to the sky
And I do believe what we rely on
When I lay it on, come get to play it on
All my life to sacrifice
Hey, oh
Listen what I say, oh
I got your— Hey, oh
Listen what I say, oh
The more I see, the less I know
The more I like to let it go
Hey oh, whoa-whoa
Deep beneath the cover of another perfect wonder
Where its so white as snow
Finally divided by a world so undecided
And theres nowhere to go
In between the cover of another perfect wonder
Where its so white as snow
Running through the field where all my tracks will be concealed
And theres nowhere to go
 I said, hey— Hey, yeah
Oh, yeah! Tell my love now
 Hey— Hey, yeah
Oh, yeah! Tell my love now
Deep beneath the cover of another perfect wonder
Where its so white as snow
Finally divided by a world so undecided
And theres nowhere to go
Deep beneath the cover of another perfect wonder
Where its so white as snow
Running through the field where all my tracks will be concealed
And theres nowhere to go
 I said, hey— Hey, yeah
Oh, yeah! Tell my love now
 Hey— Hey, yeah
Oh, yeah!
)TXT",
R"TXT(How long, how long will I slide?
Well, separate my side
I dont, I dont believe its bad
Slittin my throat, its all I ever
I heard your voice through a photograph
I thought it up and brought up the past
Once you know you can never go back
I gotta take it on the otherside
Well, centuries are what it meant to me
A cemetery where I marry the sea
The stranger things that never change my mind
I gotta take it on the otherside
Take it on the otherside
Take it on, take it on
How long, how long will I slide?
A-separate my side
I dont, I dont believe its bad
A-slittin my throat, its all I ever
Pour my life into a paper cup
The ashtrays full and Im spillin my guts
She want to know am I still a slut?
I gotta take it on the otherside
A scarlet starlet and shes in my bed
A candidate for my soul mate bled
Mmm, push the trigger and I pull the thread
I gotta take it on the otherside
Take it on the otherside
Take it on, take it on
How long, how long will I slide?
Separate my side
I dont, I dont believe its bad
A-slittin my throat, its all I ever
Turn me on, take me for a hard ride
Burn me out, leave me on the otherside
I yell and tell it that its not my friend
I tear it down, I tear it down, and then its born again
How long, how long will I slide?
Separate my side
I dont, I dont believe its bad
A-slittin my throat, its all I ever had
I dont, I dont believe it’s sad
A-slittin my throat, its all I ever
)TXT",
R"TXT(
Can I get your hand to write on?
Just a piece of leg to bite on
What a night to fly my kite on
Do you want to flash a light on?
Take a look its on display for you
Coming down, no, not today
Did you meet your fortune teller?
Get it off with no propeller
Do it up, its on with Stella
What a way to finally smell her
Picking up, but not too strong for you
Take a piece and pass it on
Fly away on my zephyr
I feel it more than ever
And in this perfect weather
Well find a place together
Fly on my wind
Rebel and a liberator
Find a way to be a skater
Rev it up to levitate her
Super friendly aviator
Take a look its on display, for you
Coming down, no not today
Fly away on my zephyr
I feel it more than ever
And in this perfect weather
Well find a place together
In the water where I center my emotion
All the world can pass me by
Fly away on my Zephyr
Well find a place together
Wo-wo-wo-wo-wo-woah, do you
Yea-yea-yea-yea-yea-yeah
Wo-wo-wo-wo-wo-woah, want to
Yea-yea-yea-yea-yea-yeah
Fly away on my zephyr
I feel it more than ever
And in this perfect weather
Well find a place together
In the water where I center my emotion
All the world can pass me by
Fly away on my Zephyr
Were going to live forever
Forever)TXT"
};
CONSOLE_APP_MAIN {
	using namespace TextLib;
	for(int i = 0; i < SCRIPT_COUNT; i++) {
		if (i != 2)
			continue;
		LOG("######## " << IntStr(i+1) << " ########");
		MultiScriptStructureSolver s;
		s.Get().SetForcedLimit(0.4);
		s.Get().Process(scripts[i]);
		LOG(s.Get().GetDebugOut());
		LOG(s.Get().GetDebugLines());
		LOG(s.Get().GetResult());
		LOG("\n\n\n");
	}
}
