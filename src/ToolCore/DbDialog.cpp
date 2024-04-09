#include "ToolCore.h"
#include "ProtectedCommon.h"

BEGIN_TEXTLIB_NAMESPACE

const Index<String>& GetCharacters() {
	static Index<String> list;
	
	if (list.IsEmpty()) {
		list.Add("The Influencer");
		list.Add("The Authority");
		list.Add("The Critic");
		list.Add("The Storyteller");
		list.Add("The Trendsetter");
		list.Add("The Jokester");
		list.Add("The Insider");
		list.Add("The Activist");
		list.Add("The Creative");
		list.Add("The Adventurer");
		list.Add("The Lifestyle Guru");
		list.Add("Non-Niche Niche");
		list.Add("The Activator");
		list.Add("The Connector");
		list.Add("The Opinionated");
		list.Add("The Educator");
		list.Add("The Lifestyle Enthusiast");
		list.Add("The Humanitarian");
		list.Add("The Newbie");
	}
	return list;
}

const Vector<ContentType>& GetTropes() {
	thread_local static Vector<ContentType> list;
	
	if (list.IsEmpty()) {
		list.Add().Set("The Hero", "Chosen One", "Reluctant Hero", "Fallen Hero");
		list.Add().Set("The Villain", "Pure Evil", "Tragic Villain", "Anti-Villain");
		list.Add().Set("The Love Interest", "Damsel in Distress", "Rebel Love Interest", "Soulmate");
		list.Add().Set("The Mentor", "Wise Old Mentor", "Reluctant Mentor", "Betrayed Mentor");
		list.Add().Set("The Sidekick", "Loyal Sidekick", "Rebellious Sidekick", "Incompetent Sidekick");
		list.Add().Set("The Rebel", "Outlaw", "Revolutionary", "Anarchist");
		list.Add().Set("The Anti-Hero", "Dark Anti-Hero", "Sarcastic Anti-Hero", "Redeemed Anti-Hero");
		list.Add().Set("The Damsel in Distress", "Helpless Damsel", "Fiery Damsel", "Rescuer Damsel");
		list.Add().Set("The Wise Old Man/Woman", "Sage", "Trickster", "Cynic");
		list.Add().Set("The Femme Fatale", "Manipulative Femme Fatale", "Redeemed Femme Fatale", "Empowered Femme Fatale");
		list.Add().Set("The Fearless Leader", "Charismatic Leader", "Ruthless Leader", "Nurturing Leader");
		list.Add().Set("The Outcast", "Loner", "Misunderstood", "Rebel");
		list.Add().Set("The Jester", "Funny Jester", "Dark Jester", "Heartfelt Jester");
		list.Add().Set("The Loner", "Sole Survivor", "Tragic Shut-In", "Recluse");
		list.Add().Set("The Innocent", "Naive Innocent", "Pure Innocent", "Sheltered Innocent");
		list.Add().Set("The Queen/King", "Power-Hungry Monarch", "Loyal Ruler", "Reluctant Ruler");
		list.Add().Set("The Badass", "Fighter", "Rebel", "Anti-Hero");
		list.Add().Set("The Chosen One", "Reluctant Chosen One", "Redeemed Chosen One", "Corrupted Chosen One");
		list.Add().Set("The Scapegoat", "Blamed Scapegoat", "Innocent Scapegoat", "Scapegoat-Turned-Villain");
		list.Add().Set("The Fallen Hero", "Betrayed Hero", "Redeemed Villain", "Anti-Villain");
		list.Add().Set("The Misunderstood", "Angsty Misunderstood", "Empathetic Misunderstood", "Rebellious Misunderstood");
		list.Add().Set("The Alpha/Beta/Omega", "Alpha Leader", "Beta Follower", "Omega Outcast");
		list.Add().Set("The Shapeshifter", "Uncontrollable Shapeshifter", "Controlled Shapeshifter", "Shapeshifter-Turned-Hero");
		list.Add().Set("The Trickster", "Clever Trickster", "Self-Serving Trickster", "Redeemed Trickster");
		list.Add().Set("The Charming Rogue", "Smooth-Talking Rogue", "Cocky Gambler", "Rogue with a Heart of Gold");
		list.Add().Set("The Underdog", "Underestimated Underdog", "Overachieving Underdog", "Underdog-Turned-Champion");
		list.Add().Set("The Joker", "Class Clown", "Dark Joker", "Rebel Joker");
		list.Add().Set("The Survivor", "Sole Survivor", "Traumatized Survivor", "Survivor Gives Up");
	}
	return list;
}

int GetTropeCount() {
	return GetTropes().GetCount();
}


VectorMap<String,Vector<String>>& GetCharacterSafeMale() {
	thread_local static VectorMap<String,Vector<String>> list;
	
	if (!list.IsEmpty()) return list;
	
	

	{auto& tc = list.Add("The Influencer");
	tc.Add("Gary Vaynerchuk (business/entrepreneurship)");
	tc.Add("Casey Neistat (vlogger/lifestyle)");
	tc.Add("Grant Cardone (sales/motivation)");
	tc.Add("Chiara Ferragni (fashion/beauty)");
	tc.Add("Peter McKinnon (photography/videography)");}

	{auto& tc = list.Add("The Authority");
	tc.Add("Tony Robbins (motivational speaker/life coach)");
	tc.Add("Tim Ferriss (entrepreneurship/productivity)");
	tc.Add("Donald Trump (politics/leadership)");
	tc.Add("Barack Obama (politics/leadership)");
	tc.Add("Malcolm Gladwell (author/behavioral psychology)");
	tc.Add("Simon Sinek (leadership/communication)");}

	{auto& tc = list.Add("The Critic");
	tc.Add("Gordon Ramsay (chef/TV personality)");
	tc.Add("Jordan Peterson (professor/author)");
	tc.Add("Seth Godin (marketing/entrepreneurship)");
	tc.Add("Niki Nakayama (chef/restaurateur)");
	tc.Add("Brian Koppelman (screenwriter/film producer)");}

	{auto& tc = list.Add("The Storyteller");
	tc.Add("Stephen King (author/screenwriter)");
	tc.Add("Tom Hanks (actor/producer)");
	tc.Add("Neil Gaiman (author/graphic novelist)");
	tc.Add("Lin-Manuel Miranda (writer/actor)");
	tc.Add("JJ Abrams (director/producer/writer)");}

	{auto& tc = list.Add("The Trendsetter");
	tc.Add("Rihanna (singer/fashion designer)");
	tc.Add("Kanye West (rapper/producer/fashion designer)");
	tc.Add("Virgil Abloh (fashion designer/creative director)");
	tc.Add("Jeffree Star (beauty influencer/entrepreneur)");
	tc.Add("Tyler, The Creator (rapper/producer/fashion designer");}

	{auto& tc = list.Add("The Jokester");
	tc.Add("Dave Chappelle (comedian/actor)");
	tc.Add("Stephen Colbert (comedian/TV host)");
	tc.Add("Ryan Reynolds (actor/comedian)");
	tc.Add("Aziz Ansari (comedian/actor)");
	tc.Add("Kevin Hart (comedian/actor)");}

	{auto& tc = list.Add("The Insider");
	tc.Add("Edward Snowden (former CIA employee/whistleblower)");
	tc.Add("Chelsea Manning (former US Army soldier/whistleblower)");
	tc.Add("Julian Assange (founder of Wikileaks/journalist)");
	tc.Add("Daniel Ellsberg (former US military analyst/whistleblower)");
	tc.Add("Eric Snowden (retired NSA employee/whistleblower)");}

	{auto& tc = list.Add("The Activist");
	tc.Add("Malala Yousafzai (education activist/Nobel Peace Prize winner)");
	tc.Add("Greta Thunberg (environmental activist)");
	tc.Add("Patrisse Cullors (co-founder of Black Lives Matter)");
	tc.Add("Martin Luther King Jr. (civil rights activist)");
	tc.Add("Mahatma Gandhi (leader of Indian independence movement)");}

	{auto& tc = list.Add("The Creative");
	tc.Add("Banksy (street artist/political activist)");
	tc.Add("Quentin Tarantino (film director/screenwriter)");
	tc.Add("Ai Weiwei (conceptual artist/activist)");
	tc.Add("David Bowie (singer/actor)");
	tc.Add("Salvador Dali (surrealist artist)");}

	{auto& tc = list.Add("The Adventurer");
	tc.Add("Bear Grylls (adventurer/TV host)");
	tc.Add("Lewis Pugh (ocean advocate/extreme swimmer)");
	tc.Add("Howard Schultz (businessman/CEO)");
	tc.Add("Alex Honnold (rock climber/adventurer)");
	tc.Add("Chris Hadfield (astronaut/author)");}

	{auto& tc = list.Add("The Lifestyle Guru");
	tc.Add("Marie Kondo (organizing consultant/TV personality)");
	tc.Add("Tony Robbins (motivational speaker/life coach)");
	tc.Add("Chrissy Teigen (TV host/model/cookbook author)");
	tc.Add("Martha Stewart (lifestyle expert/TV personality)");
	tc.Add("Tim Ferriss (entrepreneurship/productivity)");}

	{auto& tc = list.Add("Non-Niche Niche");
	tc.Add("David Beckham (former professional soccer player/model)");
	tc.Add("Oprah Winfrey (media mogul/TV host/philanthropist)");
	tc.Add("Elon Musk (entrepreneur/CEO of Tesla and SpaceX)");
	tc.Add("Ellen DeGeneres (comedian/TV host)");
	tc.Add("David Attenborough (naturalist/documentary host)");}

	{auto& tc = list.Add("The Activator");
	tc.Add("Tony Robbins (author/motivational speaker/life coach)");
	tc.Add("Malala Yousafzai (activist/Nobel Peace Prize winner)");
	tc.Add("Richard Branson (entrepreneur/CEO of Virgin Group)");
	tc.Add("Trevor Noah (comedian/TV host/activist)");
	tc.Add("Tim Ferriss (author/podcaster/lifestyle guru)");}

	{auto& tc = list.Add("The Connector");
	tc.Add("Barack Obama (former president/public speaker/author)");
	tc.Add("Simon Sinek (author/inspirational speaker/leadership expert)");
	tc.Add("Brené Brown (researcher/author/podcaster)");
	tc.Add("Bill Gates (entrepreneur/philanthropist)");
	tc.Add("Jay Shetty (former monk/podcaster/author)");}

	{auto& tc = list.Add("The Opinionated");
	tc.Add("Ben Shapiro (political commentator/commentary)");
	tc.Add("Steven Crowder (political commentator/commentary)");
	tc.Add("Joe Rogan (podcasting/politics)");
	tc.Add("Tim Pool (journalism/politics)");
	tc.Add("Tucker Carlson (political commentator/commentary)");
	tc.Add("Dave Rubin (political commentator/commentary)");
	tc.Add("Jordan Peterson (psychology/philosophy)");}
	
	{auto& tc = list.Add("The Educator");
	tc.Add("Neil deGrasse Tyson (astrophysicist/science communicator)");
	tc.Add("Tom Hanks (actor/producer/performer)");
	tc.Add("Malcolm Gladwell (author/podcaster/thought leader)");
	tc.Add("Tony Hawk (professional skateboarder/entrepreneur)");
	tc.Add("Cory Booker (politician/author/activist)");}

	{auto& tc = list.Add("The Lifestyle Enthusiast");
	tc.Add("Tim Ferriss (lifestyle design/productivity)");
	tc.Add("Lewis Howes (entrepreneurship/personal development)");
	tc.Add("Gary Vaynerchuk (entrepreneurship/marketing)");
	tc.Add("Derek Sivers (entrepreneurship/lifestyle design)");
	tc.Add("Casey Neistat (filmmaking/entrepreneurship)");
	tc.Add("Jamie Oliver (food/health)");
	tc.Add("Dave Ramsey (finance/personal development)");
	tc.Add("Tony Robbins (personal development/motivation)");}
	
	{auto& tc = list.Add("The Humanitarian");
	tc.Add("Mahatma Gandhi (activist/leader)");
	tc.Add("Leonardo DiCaprio (actor/environmental activist/philanthropist)");
	tc.Add("Paul Walker (actor/humanitarian)");
	tc.Add("Justin Trudeau (politician/activist)");
	tc.Add("Will Smith (actor/philanthropist/rapper)");}

	{auto& tc = list.Add("The Newbie");
	tc.Add("Justin Bieber (singer)");
	tc.Add("Shawn Mendes (singer/songwriter/activist)");
	tc.Add("Joe Jonas (singer/songwriter/actor)");
	tc.Add("John Boyega (actor/activist) ");
	tc.Add("Timothée Chalamet (actor/activist)");}
	
	ASSERT(GetCharacters().GetCount() == list.GetCount());
	return list;
}

VectorMap<String,Vector<String>>& GetCharacterSafeFemale() {
	thread_local static VectorMap<String,Vector<String>> list;
	
	if (!list.IsEmpty()) return list;
    
	{auto& tc = list.Add("The Influencer");
	tc.Add("Sarah Huckabee Sanders (former White House press secretary)");
	tc.Add("Nikki Haley (former UN ambassador/former governor)");
	tc.Add("Ann Coulter (author/commentator) ");
	tc.Add("Mary Matalin (political consultant/commentator) ");
	tc.Add("Condoleezza Rice (former secretary of state/political scientist)");}

	{auto& tc = list.Add("The Authority");
	tc.Add("Elizabeth Dole (former senator/cabinet member)");
	tc.Add("Kellyanne Conway (former White House counselor)");
	tc.Add("Condoleezza Rice (former secretary of state/political scientist)");
	tc.Add("Carly Fiorina (former business executive/presidential candidate)");
	tc.Add("Susan Collins (senator)");}

	{auto& tc = list.Add("The Critic");
	tc.Add("Megyn Kelly (journalist/commentator) ");
	tc.Add("Laura Ingraham (political commentator/talk show host) ");
	tc.Add("Michelle Malkin (political commentator/author) ");
	tc.Add("Candace Owens (political commentator/activist)");
	tc.Add("Tomi Lahren (political commentator/activist)");}

	{auto& tc = list.Add("The Storyteller");
	tc.Add("Laura Bush (former first lady/educator)");
	tc.Add("Peggy Noonan (author/columnist/speechwriter)");
	tc.Add("Jenna Ellis (political commentator/author/lawyer)");
	tc.Add("Sarah Palin (former governor/vp candidate/author)");
	tc.Add("Mercedes Schlapp (political commentator/former White House director of strategic communications)");}

	{auto& tc = list.Add("The Trendsetter");
	tc.Add("Ivanka Trump (advisor to the president)");
	tc.Add("Betsy DeVos (secretary of education/former businesswoman)");
	tc.Add("Lara Trump (political consultant/TV producer)");
	tc.Add("Jaelen King (social media strategist/activist)");
	tc.Add("Angela \"Bay\" Buchanan (political commentator/strategist)");}

	{auto& tc = list.Add("The Jokester");
	tc.Add("Heather Nauert (former State Department spokeswoman/TV host)");
	tc.Add("Dana Perino (former White House press secretary/political commentator)");
	tc.Add("Allie Beth Stuckey (political commentator/comedian)");
	tc.Add("Gretchen Carlson (TV anchor/women's rights advocate)");
	tc.Add("Nia-Malika Henderson (senior political reporter/correspondent)");}

	{auto& tc = list.Add("The Insider");
	tc.Add("Hope Hicks (former White House communications director)");
	tc.Add("Elise Stefanik (congresswoman)");
	tc.Add("Mary-Kate Fisher (political correspondent/journalist)");
	tc.Add("Jenna Bush Hager (journalist/author/TV personality)");
	tc.Add("Candice Miller (congresswoman/former Michigan secretary of state)");}

	{auto& tc = list.Add("The Activist");
	tc.Add("Kimberly Guilfoyle (political commentator/activist)");
	tc.Add("Candace Owens (political commentator/activist)");
	tc.Add("Nikki Haley (former UN ambassador/governor)");
	tc.Add("Tomi Lahren (political commentator/activist)");
	tc.Add("Marsha Blackburn (senator/activist)");}

	{auto& tc = list.Add("The Creative");
	tc.Add("Melissa Francis (TV host/author/actress)");
	tc.Add("Inez Stepman (senior policy analyst/author/activist)");
	tc.Add("Lauren Simonetti (TV reporter/news anchor)");
	tc.Add("Mindy Finn (political strategist/digital marketer)");
	tc.Add("Meghan McCain (TV host/columnist/author)");}


	{auto& tc = list.Add("The Adventurer");
	tc.Add("Joni Ernst (senator/military veteran)");
	tc.Add("Betsy DeVos (secretary of education/former businesswoman)");
	tc.Add("Sarah Palin (former governor/VP candidate/author)");
	tc.Add("Carly Fiorina (former business executive/presidential candidate)");
	tc.Add("Nikki Haley (former UN ambassador/governor)");}

	{auto& tc = list.Add("The Lifestyle Guru");
	tc.Add("Ivanka Trump (advisor to the president)");
	tc.Add("Melania Trump (First Lady/entrepreneur)");
	tc.Add("Elaine Chao (secretary of transportation/former secretary of labor)");
	tc.Add("Kayleigh McEnany (former White House press secretary/political commentator)");
	tc.Add("Leslie Sanchez (political commentator/author/strategist)");}

	{auto& tc = list.Add("Non-Niche Niche");
	tc.Add("Anna Paulina Luna (political commentator/activist)");
	tc.Add("Aja Smith (political commentator/activist/speaker)");
	tc.Add("Ashley Pratte (political commentator/strategist)");
	tc.Add("Buffy Wicks (political strategist/activist)");
	tc.Add("Sarah Chamberlain (CEO/activist/strategist)");}

	{auto& tc = list.Add("The Activator");
	tc.Add("Lara Trump (political consultant/TV producer)");
	tc.Add("Candice Miller (congresswoman/former Michigan secretary of state)");
	tc.Add("Jaelen King (social media strategist/activist)");
	tc.Add("Kelli Ward (congresswoman/political commentator)");
	tc.Add("Marsha Blackburn (senator/activist)");}

	{auto& tc = list.Add("The Connector");
	tc.Add("Nikki Haley (former UN ambassador/governor)");
	tc.Add("Elaine Chao (secretary of transportation/former secretary of labor)");
	tc.Add("Sarah Huckabee Sanders (former White House press secretary)");
	tc.Add("Maria Bartiromo (journalist/TV anchor)");
	tc.Add("Liz Cheney (congresswoman/lawyer/political commentator)");}

	{auto& tc = list.Add("The Opinionated");
	tc.Add("Tomi Lahren (political commentator/activist)");
	tc.Add("Kimberly Guilfoyle (political commentator/activist)");
	tc.Add("Candace Owens (political commentator/activist)");
	tc.Add("Ann Coulter (author/commentator)");
	tc.Add("S.E. Cupp (political commentator/TV host)");}

	{auto& tc = list.Add("The Educator");
	tc.Add("Betsy DeVos (secretary of education/former businesswoman)");
	tc.Add("Candice Miller (congresswoman/former Michigan secretary of state)");
	tc.Add("Nikki Haley (former UN ambassador/governor)");
	tc.Add("Carly Fiorina (former business executive/presidential candidate)");
	tc.Add("Elaine Chao (secretary of transportation/former secretary of labor)");}

	{auto& tc = list.Add("The Lifestyle Enthusiast");
	tc.Add("Ivanka Trump (advisor to the president)");
	tc.Add("Sarah Palin (former governor/VP candidate/author)");
	tc.Add("Nikki Haley (former UN ambassador/governor)");
	tc.Add("Melania Trump (First Lady/entrepreneur)");
	tc.Add("Lara Trump (political consultant/TV producer)");}

	{auto& tc = list.Add("The Humanitarian");
	tc.Add("Ivanka Trump (advisor to the president)");
	tc.Add("Betsy DeVos (secretary of education/former businesswoman)");
	tc.Add("Liz Cheney (congresswoman/lawyer/political commentator)");
	tc.Add("Heather Nauert (former State Department spokesperson/TV host)");
	tc.Add("Kayleigh McEnany (former White House press secretary/political commentator)");}

	{auto& tc = list.Add("The Newbie");
	tc.Add("Madison Cawthorn (congressman/youngest in Congress)");
	tc.Add("Marjorie Taylor Greene (congresswoman/controversial figure)");
	tc.Add("Kat Cammack (congresswoman/youngest woman in Congress)");
	tc.Add("Nicole Malliotakis (congresswoman/first Greek-American woman elected to Congress)");
	tc.Add("Young Kim (congresswoman/first Korean-American Republican woman elected to Congress)");}
	
	ASSERT(GetCharacters().GetCount() == list.GetCount());
	return list;
}

VectorMap<String,Vector<String>>& GetCharacterUnsafeMale() {
	thread_local static VectorMap<String,Vector<String>> list;
	
	if (!list.IsEmpty()) return list;
	
	{auto& tc = list.Add("The Influencer");
	tc.Add("Kanye West (rapper/producer/fashion designer)");
	tc.Add("Elon Musk (entrepreneur/inventor/CEO)");
	tc.Add("Conor McGregor (MMA fighter/entrepreneur)");
	tc.Add("Howard Stern (radio personality/author/actor)");
	tc.Add("Art Bell (radio host)");}

	{auto& tc = list.Add("The Authority");
	tc.Add("Alex Jones (radio host/conspiracy theorist)");
	tc.Add("Donald TV-Boss (former US President/lawyer/author)");
	tc.Add("Joe Rogan (comedian/podcast host/actor)");
	tc.Add("Russell Brand (comedian/actor/author)");
	tc.Add("Tucker Carlson (political commentator/TV host/editor)");}

	{auto& tc = list.Add("The Critic");
	tc.Add("Simon Cowell (TV producer/judge/entertainment mogul)");
	tc.Add("Gordon Ramsay (celebrity chef/restaurateur/TV personality)");
	tc.Add("Simon Cowell (TV producer/judge/entertainment mogul)");
	tc.Add("Howard Stern (radio personality/author/actor)");
	tc.Add("Piers Morgan (TV presenter/journalist/author)");}

	{auto& tc = list.Add("The Storyteller");
	tc.Add("Morgan Freeman (actor/narrator/producer)");
	tc.Add("Quentin Tarantino (filmmaker/actor/screenwriter)");
	tc.Add("Neil Gaiman (author/screenwriter/comic book writer)");
	tc.Add("David Sedaris (humorist/author/radio contributor)");
	tc.Add("Wes Anderson (filmmaker/screenwriter/producer)");}

	{auto& tc = list.Add("The Trendsetter");
	tc.Add("Kanye West (rapper/producer/fashion designer)");
	tc.Add("Jeffree Star (makeup artist/entrepreneur/YouTube personality)");
	tc.Add("David Beckham (former professional soccer player/entrepreneur/model)");
	tc.Add("Andrew Yang (entrepreneur/politician/author)");
	tc.Add("David Dobrik (YouTube personality/actor/documentary filmmaker)");}

	{auto& tc = list.Add("The Jokester");
	tc.Add("Kevin Hart (comedian/actor/producer)");
	tc.Add("Seth Rogen (actor/comedian/screenwriter)");
	tc.Add("Dave Chappelle (comedian/actor/screenwriter)");
	tc.Add("Ricky Gervais (comedian/actor/screenwriter)");
	tc.Add("Bill Burr (comedian/actor/producer)");}

	{auto& tc = list.Add("The Insider");
	tc.Add("Julian Assange (journalist/hacker/activist)");
	tc.Add("Edward Snowden (whistleblower/activist/former CIA employee)");
	tc.Add("Mark Felt (former FBI agent/Deep Throat informant)");
	tc.Add("Bob Woodward (journalist/author)");
	tc.Add("Julian Edelman (professional football player/author)");}

	{auto& tc = list.Add("The Activist");
	tc.Add("Colin Kaepernick (former professional football player/activist)");
	tc.Add("Martin Luther King Jr. (activist/minister/leader of the Civil Rights Movement)");
	tc.Add("Greta Thunberg (environmental activist)");
	tc.Add("Malcolm X (human rights activist/minister/leader of the Black Power movement)");
	tc.Add("Harvey Milk (gay rights activist/politician)");}

	{auto& tc = list.Add("The Creative");
	tc.Add("Wes Anderson (filmmaker/screenwriter/producer)");
	tc.Add("Salvador Dali (artist/surrealist)");
	tc.Add("Andy Warhol (artist/filmmaker)");
	tc.Add("Damien Hirst (artist)");
	tc.Add("Banksy (street artist/filmmaker/activist)");}

	{auto& tc = list.Add("The Adventurer");
	tc.Add("Sir Edmund Hillary (mountaineer/explorer/philanthropist)");
	tc.Add("Bear Grylls (adventurer/TV presenter/writer)");
	tc.Add("Steve Irwin (wildlife expert/TV personality/conservationist)");
	tc.Add("Philippe Petit (tightrope walker/high-wire artist)");
	tc.Add("Felix Baumgartner (austrian skydiver/base jumper)");}

	{auto& tc = list.Add("The Lifestyle Guru");
	tc.Add("Tony Robbins (motivational speaker/author/entrepreneur)");
	tc.Add("Tim Ferriss (entrepreneur/author/podcast host)");
	tc.Add("Deepak Chopra (spiritual teacher/author)");
	tc.Add("Dave Asprey (entrepreneur/author/health advocate)");
	tc.Add("Gary Vaynerchuk (entrepreneur/investor/author)");}

	{auto& tc = list.Add("Non-Niche Niche");
	tc.Add("Shia LaBeouf (actor/artist/filmmaker)");
	tc.Add("James Franco (actor/director/producer)");
	tc.Add("Jaden Smith (actor/musician/entrepreneur)");
	tc.Add("Jared Leto (actor/musician/director)");
	tc.Add("Kanye West (rapper/producer/fashion designer)");}

	{auto& tc = list.Add("The Activator");
	tc.Add("Steve Jobs (entrepreneur/CEO/founder of Apple Inc.)");
	tc.Add("Jeff Bezos (entrepreneur/CEO/founder of Amazon)");
	tc.Add("Richard Branson (entrepreneur/CEO/founder of Virgin Group)");
	tc.Add("Mark Zuckerberg (entrepreneur/CEO/co-founder of Facebook)");
	tc.Add("Elon Musk (entrepreneur/inventor/CEO)");}

	{auto& tc = list.Add("The Connector");
	tc.Add("Tim Ferriss (entrepreneur/author/podcast host)");
	tc.Add("Kevin Bacon (actor/musical artist)");
	tc.Add("Malcolm Gladwell (author/journalist/public speaker)");
	tc.Add("Gary Vaynerchuk (entrepreneur/investor/author)");
	tc.Add("Oprah Winfrey (media executive/actress/talk show host)");}

	{auto& tc = list.Add("The Opinionated");
	tc.Add("Bill O'Reilly (political commentator/TV host/author)");
	tc.Add("Tucker Carlson (political commentator/TV host/editor)");
	tc.Add("Milo Yiannopoulos (journalist/author/political commentator)");
	tc.Add("Ben Shapiro (conservative political commentator/columnist/author)");
	tc.Add("Rush Limbaugh (conservative radio host/author)");}

	{auto& tc = list.Add("The Educator");
	tc.Add("Neil deGrasse Tyson (astrophysicist/science communicator/author)");
	tc.Add("Bill Nye (mechanical engineer/science educator/TV host)");
	tc.Add("Noam Chomsky (linguist/philosopher/political activist)");
	tc.Add("Carl Sagan (astronomer/astrophysicist/author)");
	tc.Add("Jordan Peterson (psychologist/author) ");}

	{auto& tc = list.Add("The Lifestyle Enthusiast");
	tc.Add("Jeffree Star (makeup artist/entrepreneur/YouTube personality)");
	tc.Add("David Beckham (former professional soccer player/entrepreneur/model)");
	tc.Add("Scott Disick (TV personality/entrepreneur/model)");
	tc.Add("Jason Momoa (actor/producer/director)");
	tc.Add("Dwayne Johnson (actor/producer/wrestler) ");}

	{auto& tc = list.Add("The Humanitarian");
	tc.Add("Malala Yousafzai (noble peace prize laureate/activist/author)");
	tc.Add("Mahatma Gandhi (political leader/activist)");
	tc.Add("Martin Luther King Jr. (activist/minister/leader of the Civil Rights Movement)");
	tc.Add("Mother Teresa (Catholic nun/missionary/humanitarian)");
	tc.Add("Elie Wiesel (Nobel Peace Prize laureate/activist/author) ");}

	{auto& tc = list.Add("The Newbie");
	tc.Add("Timothee Chalamet (actor)");
	tc.Add("Tom Holland (actor)");
	tc.Add("Noah Centineo (actor)");
	tc.Add("Ansel Elgort (actor)");
	tc.Add("Jacob Elordi (actor)");}
	
	ASSERT(GetCharacters().GetCount() == list.GetCount());
	return list;
}

VectorMap<String,Vector<String>>& GetCharacterUnsafeFemale() {
	thread_local static VectorMap<String,Vector<String>> list;
	
	if (!list.IsEmpty()) return list;
		{auto& tc = list.Add("The Influencer");
	tc.Add("Cardi B (rapper/actress/entrepreneur)");
	tc.Add("Kim Kardashian (reality TV star/businesswoman)");
	tc.Add("Huda Kattan (beauty blogger/entrepreneur)");
	tc.Add("Brené Brown (researcher/author/podcaster)");
	tc.Add("Naomi Klein (activist/journalist/author)");}

	{auto& tc = list.Add("The Authority");
	tc.Add("Oprah Winfrey (media mogul/philanthropist/actress)");
	tc.Add("Sheryl Sandberg (business executive/author)");
	tc.Add("Malala Yousafzai (human rights activist/Nobel Prize laureate)");
	tc.Add("Jacinda Ardern (prime minister of New Zealand)");
	tc.Add("Angela Merkel (Chancellor of Germany) ");}

	{auto& tc = list.Add("The Critic");
	tc.Add("AOC (political activist/congresswoman)");
	tc.Add("Greta Thunberg (environmental activist)");
	tc.Add("Roxane Gay (author/activist)");
	tc.Add("Chrissy Teigen (model/author/TV personality/social media critic)");
	tc.Add("Amanda Seales (actress/comedian/activist)");}

	{auto& tc = list.Add("The Storyteller");
	tc.Add("Chimamanda Ngozi Adichie (author/feminist)");
	tc.Add("J.K. Rowling (author/screenwriter/philanthropist)");
	tc.Add("Mindy Kaling (actress/producer/writer)");
	tc.Add("Lizzo (singer/rapper)");
	tc.Add("Ava DuVernay (filmmaker/activist)");}

	{auto& tc = list.Add("The Trendsetter");
	tc.Add("Rihanna (singer/entrepreneur/fashion designer)");
	tc.Add("Kylie Jenner (businesswoman/entrepreneur/influencer)");
	tc.Add("Alexa Chung (model/TV presenter/fashion designer)");
	tc.Add("Halsey (singer/songwriter)");
	tc.Add("Anna Wintour (editor-in-chief of Vogue)");}

	{auto& tc = list.Add("The Jokester");
	tc.Add("Tiffany Haddish (actress/comedian)");
	tc.Add("Ali Wong (comedian/actress/writer)");
	tc.Add("Phoebe Robinson (comedian/actress)");
	tc.Add("Amy Schumer (comedian/actress/writer)");
	tc.Add("Aidy Bryant (comedian/actress)");}

	{auto& tc = list.Add("The Insider");
	tc.Add("Beyoncé (singer/songwriter/actress/entrepreneur)");
	tc.Add("Serena Williams (professional tennis player/entrepreneur)");
	tc.Add("Jennifer Lawrence (actress/philanthropist)");
	tc.Add("Meghan Markle (actress/royal family member)");
	tc.Add("Reese Witherspoon (actress/producer/entrepreneur)");}

	{auto& tc = list.Add("The Activist");
	tc.Add("Amandla Stenberg (actress/activist)");
	tc.Add("Jane Fonda (actress/activist)");
	tc.Add("Tarana Burke (activist/founder of the Me Too movement)");
	tc.Add("Diane Guerrero (actress/immigrant rights activist)");
	tc.Add("Gloria Steinem (feminist/social and political activist)");}

	{auto& tc = list.Add("The Creative");
	tc.Add("Solange Knowles (singer/songwriter/actress)");
	tc.Add("Ava DuVernay (filmmaker/activist)");
	tc.Add("Shonda Rhimes (TV producer/screenwriter)");
	tc.Add("Roxane Gay (author/activist)");
	tc.Add("Carrie Brownstein (musician/actress/writer/director)");}

	{auto& tc = list.Add("The Adventurer");
	tc.Add("Bear Grylls (adventurer/TV presenter)");
	tc.Add("Amelia Earhart (aviator/author)");
	tc.Add("Alex Honnold (free solo rock climber)");
	tc.Add("Cheryl Strayed (author/adventurer)");
	tc.Add("Samantha Brown (TV host/travel expert)");}

	{auto& tc = list.Add("The Lifestyle Guru");
	tc.Add("Marie Kondo (organizing consultant/author)");
	tc.Add("Gwyneth Paltrow (actress/entrepreneur/wellness guru)");
	tc.Add("Joanna Gaines (designer/TV personality/author)");
	tc.Add("Jessica Alba (actress/entrepreneur/CEO of The Honest Company)");
	tc.Add("Kris Carr (author/wellness coach/filmmaker)");}

	{auto& tc = list.Add("Non-Niche Niche");
	tc.Add("Tiffany Haddish (actress/comedian)");
	tc.Add("Chrissy Teigen (model/author/TV personality/social media influencer)");
	tc.Add("Mindy Kaling (actress/producer/writer)");
	tc.Add("Ali Wong (comedian/actress/writer)");
	tc.Add("Aidy Bryant (comedian/actress)");}

	{auto& tc = list.Add("The Activator");
	tc.Add("Alexandria Ocasio-Cortez (political activist/congresswoman)");
	tc.Add("Greta Thunberg (environmental activist)");
	tc.Add("Tarana Burke (activist/founder of the Me Too movement)");
	tc.Add("Jane Fonda (actress/activist)");
	tc.Add("Malala Yousafzai (human rights activist/Nobel Prize laureate)");}

	{auto& tc = list.Add("The Connector");
	tc.Add("Oprah Winfrey (media mogul/philanthropist/actress)");
	tc.Add("Sheryl Sandberg (business executive/author)");
	tc.Add("Arianna Huffington (media mogul/author/businesswoman)");
	tc.Add("Melinda Gates (philanthropist/businesswoman)");
	tc.Add("Sophia Amoruso (entrepreneur/author)");}

	{auto& tc = list.Add("The Opinionated");
	tc.Add("Roxane Gay (author/activist)");
	tc.Add("Naomi Klein (activist/journalist/author)");
	tc.Add("Jacinda Ardern (prime minister of New Zealand)");
	tc.Add("AOC (political activist/congresswoman)");
	tc.Add("Amanda Seales (actress/comedian/activist)");}

	{auto& tc = list.Add("The Educator");
	tc.Add("Brené Brown (researcher/author/podcaster)");
	tc.Add("Chimamanda Ngozi Adichie (author/feminist)");
	tc.Add("Malala Yousafzai (human rights activist/Nobel Prize laureate)");
	tc.Add("Jane Goodall (primatologist/anthropologist/conservationist)");}

	{auto& tc = list.Add("The Lifestyle Enthusiast");
	tc.Add("Jamie Oliver (celebrity chef/activist)");
	tc.Add("Goop (wellness and lifestyle brand founded by Gwyneth Paltrow)");
	tc.Add("Ina Garten (author/TV host/entrepreneur)");
	tc.Add("Kourtney Kardashian (reality TV star/entrepreneur/wellness advocate)");
	tc.Add("Marie Forleo (author/life coach/entrepreneur)");}

	{auto& tc = list.Add("The Humanitarian");
	tc.Add("Angelina Jolie (actress/humanitarian)");
	tc.Add("Princess Diana (humanitarian/royal family member)");
	tc.Add("Melinda Gates (philanthropist/businesswoman)");
	tc.Add("Malala Yousafzai (human rights activist/Nobel Prize laureate)");
	tc.Add("Ellen Degeneres (comedian/TV host/philanthropist)");}

	{auto& tc = list.Add("The Newbie");
	tc.Add("Zendaya (actress/singer)");
	tc.Add("Yara Shahidi (actress/activist)");
	tc.Add("Maisie Williams (actress/activist)");
	tc.Add("Lili Reinhart (actress/activist)");
	tc.Add("Millie Bobby Brown (actress/activist)");}
	
	ASSERT(GetCharacters().GetCount() == list.GetCount());
	return list;
}

VectorMap<String,Vector<String>>& GetCharacterSafe(bool gender) {
	if (!gender)
		return GetCharacterSafeMale();
	else
		return GetCharacterSafeFemale();
}

VectorMap<String,Vector<String>>& GetCharacterUnsafe(bool gender) {
	if (!gender)
		return GetCharacterUnsafeMale();
	else
		return GetCharacterUnsafeFemale();
}

VectorMap<String,Vector<String>>& GetCharacterTropes(bool unsafe, bool gender) {
	if (!unsafe)
		return GetCharacterSafe(gender);
	else
		return GetCharacterUnsafe(gender);
}


END_TEXTLIB_NAMESPACE
