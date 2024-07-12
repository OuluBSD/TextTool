#include <Core/Core.h>
#include <TextLib/TextLib.h>
using namespace Upp;

CONSOLE_APP_MAIN {
	using namespace TextLib;
	MultiScriptStructureSolver s;
	s.Get().Process(
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
)TXT");
	LOG(s.Get().GetDebugLines());
	LOG(s.Get().GetResult());
}
