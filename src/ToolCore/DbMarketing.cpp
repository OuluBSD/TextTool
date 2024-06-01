#include "ToolCore.h"
#include "ProtectedCommon.h"

BEGIN_TEXTLIB_NAMESPACE


const Index<String>& GetPersuasiveElements() {
	static Index<String> list;
	if (list.IsEmpty()) {
		list.Add("Emotion");
		list.Add("Empathy");
		list.Add("Logos");
		list.Add("Ethos");
		list.Add("Narrative");
		list.Add("Anecdotes");
		list.Add("Storytelling");
		list.Add("Passion");
		list.Add("Imagery");
		list.Add("Urgency");
		list.Add("Authority");
		list.Add("Credibility");
		list.Add("Satisfaction");
		list.Add("Word Play");
		list.Add("Metaphors");
		list.Add("Humor");
		list.Add("Exaggeration");
		list.Add("(Maximization of) Benefits");
		list.Add("Facts");
		list.Add("Evidence");
		list.Add("Desire");
		list.Add("Contrast");
		list.Add("Experience");
		list.Add("Cause & Effect empathy");
	}
	return list;
}

const Vector<ContentType>& GetCallToActions() {
	thread_local static Vector<ContentType> list;
	
	if (list.IsEmpty()) {
		list.Add().Set("Invest", "We want you!", "Making your money go further", "The Finer Details of Investing", "Investment Made Simple");
		list.Add().Set("Grow", "Take your business to the next level", "Leadership", "Strategy", "Make your mark");
		list.Add().Set("Advertise", "Synthesizing Impactful Ideas", "Persuasion Millions Will See", "Expansive Your Traffic", "Corner Your Market");
		list.Add().Set("Share Feedback", "Say what you think, and share your opinions", "Casting Your Stones at the Throne", "Praise like you mean it", "Comment Rebirth");
		list.Add().Set("Discuss", "Let's talk!", "Heralding Your Opinion", "Not afraid to say what matters", "The True You");
		list.Add().Set("View Ads", "Products galore", "We Anything", "Rock the stuff", "Admirers Revolutionized");
		list.Add().Set("Refer/Share", "The more, the merrier", "Earn While Spreading", "Rewards for being a great friend", "Spread the Pereece love!");
		list.Add().Set("Get a Freebie", "Too Good to Pass Up", "Value is !== value paid.", "Freeness to Landing Page", "Relish the Freebie Venue");
		list.Add().Set("Remind Me!", "I Need a Nudge", "I'll Do Better Business", "We'll Call When A Better Time", "Bringing It Together");
		list.Add().Set("Book an appointment", "Enter Our Schedule", "Establish Meeting Grounds", "Hhornwinning Style & Billboard Actions", "3 Things We Want");
		list.Add().Set("Order Books", "Order More Tack", "Premier Quality Only", "Seeing You Success", "The Bookmarket Family");
		list.Add().Set("Download Content", "Re-Send to You KompiK", "Illustrious Download Vendors", "Most Popular Font", "Ideal Contentized");
		list.Add().Set("Unleash Your Creativity", "Let Your Fingers Do the Talking", "Writing And Advertising Deals", "Our #1 Abacabaca Village", "Crank Up Creative Mojo");
		list.Add().Set("Reserve Your Space", "World Extended", "Keeping It Your Way!", "Perormers Edge Bravo", "A Voice in the Audience Called");
		list.Add().Set("Sign Up Now!", "Right Now", "We Do Ensure Raw", "A Deaf Name", "The Quality Life for You");
		list.Add().Set("Subscribe", "Down, But We Will Be Heard", "Fates Join", "The Consummate Ideationist", "Subscribed For DoN Certification");
		list.Add().Set("Order Anything", "Great Deals On Anything", "We Build Repercussions", "Purchase Anything and Ill Goto Child Services", "Order Our Rental On PriPipes");
		list.Add().Set("Get Our Monthly Tips Email", "Pristine Tips", "Multiply Your Audience", "IFTTT Code", "Receive Your Ideas Bonus");
		list.Add().Set("Get a {{COMPANY}} Reading List", "What Books Many Should Read", "Without Quickreview", "Activate Ebook Deals", "Skip the Market To That Extend");
		list.Add().Set("Check Out The {{COMPANY}} Webstore", "Enjoy Our Great IT Deals", "Here to Stay", "The Price You Pay On Any Corner Netwarden", "Beauty Starts Here, Beauty Extended.");
	}
	return list;
}

int GetGetCallToActionCount() {
	return GetCallToActions().GetCount();
}


VectorMap<String,Vector<String>>& GetTriggerSafeMale() {
	thread_local static VectorMap<String,Vector<String>> list;
	
	if (!list.IsEmpty()) return list;
	
	{auto& tc = list.Add("Invest");
    tc.Add("Warren Buffett");
    tc.Add("Elon Musk");
    tc.Add("Tony Robbins");
    tc.Add("Robert Kiyosaki");
    tc.Add("Tim Ferriss");}

    {auto& tc = list.Add("Grow");
    tc.Add("Richard Branson");
    tc.Add("Jeff Bezos");
    tc.Add("Steve Jobs");
    tc.Add("Mark Cuban");
    tc.Add("Bill Gates");}

    {auto& tc = list.Add("Advertise");
    tc.Add("Don Draper");
    tc.Add("David Ogilvy");
    tc.Add("Gary Vaynerchuk");
    tc.Add("Neil Patel");
    tc.Add("Seth Godin");}

    {auto& tc = list.Add("Share Feedback");
    tc.Add("Simon Sinek");
    tc.Add("Malcolm Gladwell");
    tc.Add("Daniel Pink");
    tc.Add("Guy Kawasaki");
    tc.Add("Tom Peters");}

    {auto& tc = list.Add("Discuss");
    tc.Add("Tim Draper");
    tc.Add("Tim Cook");
    tc.Add("Tim Berners-Lee");
    tc.Add("Tim Armstrong");
    tc.Add("Tim O'Reilly");}

    {auto& tc = list.Add("View Ads");
    tc.Add("David Beckham");
    tc.Add("Michael Jordan");
    tc.Add("Cristiano Ronaldo");
    tc.Add("Lionel Messi");
    tc.Add("LeBron James");}

    {auto& tc = list.Add("Refer/Share");
    tc.Add("Gary Keller");
    tc.Add("Grant Cardone");
    tc.Add("Brian Tracy");
    tc.Add("Jay Abraham");
    tc.Add("Zig Ziglar");}

    {auto& tc = list.Add("Get a Freebie");
    tc.Add("Oprah Winfrey");
    tc.Add("Ellen DeGeneres");
    tc.Add("David Copperfield");
    tc.Add("Jimmy Fallon");
    tc.Add("Tony Hawk");}

    {auto& tc = list.Add("Remind Me!");
    tc.Add("Tim Horton");
    tc.Add("Tim Gunn");
    tc.Add("Tim Conway");
    tc.Add("Tim Meadows");
    tc.Add("Tim Allen");}

    {auto& tc = list.Add("Book an appointment");
    tc.Add("Tony Hawk");
    tc.Add("Tim Ferriss");
    tc.Add("Tim Robbins");
    tc.Add("Tim Berners-Lee");
    tc.Add("Tim O'Reilly");}

    {auto& tc = list.Add("Order Books");
    tc.Add("Stephen King");
    tc.Add("James Patterson");
    tc.Add("J.K. Rowling");
    tc.Add("John Grisham");
    tc.Add("Dan Brown");}

    {auto& tc = list.Add("Download Content");
    tc.Add("Elon Musk");
    tc.Add("Tim Ferriss");
    tc.Add("Bill Gates");
    tc.Add("Jeff Bezos");
    tc.Add("Peter Thiel");}

    {auto& tc = list.Add("Unleash Your Creativity");
    tc.Add("Sir Ken Robinson");
    tc.Add("Austin Kleon");
    tc.Add("Elizabeth Gilbert");
    tc.Add("Seth Godin");
    tc.Add("Tom Kelley");}

    {auto& tc = list.Add("Reserve Your Space");
    tc.Add("Gary Vaynerchuk");
    tc.Add("Tony Robbins");
    tc.Add("Richard Branson");
    tc.Add("Tim Draper");
    tc.Add("Tim Armstrong");}

    {auto& tc = list.Add("Sign Up Now!");
    tc.Add("Robert Kiyosaki");
    tc.Add("T. Harv Eker");
    tc.Add("Brian Tracy");
    tc.Add("Darren Hardy");
    tc.Add("Jack Canfield");}

    {auto& tc = list.Add("Subscribe");
    tc.Add("Tim Ferriss");
    tc.Add("Gretchen Rubin");
    tc.Add("Lewis Howes");
    tc.Add("James Altucher");
    tc.Add("Chris Guillebeau");}

    {auto& tc = list.Add("Order Anything");
    tc.Add("Elon Musk");
    tc.Add("Jeff Bezos");
    tc.Add("Tony Robbins");
    tc.Add("Gary Vaynerchuk");
    tc.Add("Seth Godin");}

    {auto& tc = list.Add("Get Our Monthly Tips Email");
    tc.Add("Tim Ferriss");
    tc.Add("Ramit Sethi");
    tc.Add("Tim Harford");
    tc.Add("Malcolm Gladwell");
    tc.Add("Daniel Pink");}

    {auto& tc = list.Add("Get a {{COMPANY}} Reading List");
    tc.Add("Jack Welch");
    tc.Add("Michael Porter");
    tc.Add("Jim Collins");
    tc.Add("Thomas Friedman");
    tc.Add("Warren Buffett");}

    {auto& tc = list.Add("Check Out The {{COMPANY}} Webstore");
    tc.Add("Larry Page");
    tc.Add("Sergey Brin");
    tc.Add("Steve Jobs");
    tc.Add("Soichiro Honda");
    tc.Add("Phil Knight");}
	
	return list;
}

VectorMap<String,Vector<String>>& GetTriggerSafeFemale() {
	thread_local static VectorMap<String,Vector<String>> list;
	
	if (!list.IsEmpty()) return list;
	
	{auto& tc = list.Add("Invest");
    tc.Add("Oprah Winfrey");
    tc.Add("Sheryl Sandberg");
    tc.Add("Arianna Huffington");
    tc.Add("Melinda Gates");
    tc.Add("Bethenny Frankel");}

    {auto& tc = list.Add("Grow");
    tc.Add("Indra Nooyi");
    tc.Add("Beyoncé");
    tc.Add("Michelle Obama");
    tc.Add("Marissa Mayer");
    tc.Add("Elena Cardone");}

    {auto& tc = list.Add("Advertise");
    tc.Add("Kim Kardashian");
    tc.Add("Ellen DeGeneres");
    tc.Add("Kylie Jenner");
    tc.Add("Sofia Vergara");
    tc.Add("Tyra Banks");}

    {auto& tc = list.Add("Share Feedback");
    tc.Add("Beyoncé");
    tc.Add("Jennifer Lopez");
    tc.Add("Taylor Swift");
    tc.Add("Angelina Jolie");
    tc.Add("Lady Gaga");}

    {auto& tc = list.Add("Discuss");
    tc.Add("Amy Schumer");
    tc.Add("Mindy Kaling");
    tc.Add("Chelsea Handler");
    tc.Add("Tiffany Haddish");
    tc.Add("Issa Rae");}

    {auto& tc = list.Add("View Ads");
    tc.Add("P!nk");
    tc.Add("Jennifer Aniston");
    tc.Add("Blake Lively");
    tc.Add("Jennifer Lawrence");
    tc.Add("Jennifer Garner");}

    {auto& tc = list.Add("Refer/Share");
    tc.Add("Karlie Kloss");
    tc.Add("Gigi Hadid");
    tc.Add("Chrissy Teigen");
    tc.Add("Kate Hudson");
    tc.Add("Reese Witherspoon");}

    {auto& tc = list.Add("Get a Freebie");
    tc.Add("Katy Perry");
    tc.Add("Rihanna");
    tc.Add("Selena Gomez");
    tc.Add("Demi Lovato");
    tc.Add("Hailey Bieber");}

    {auto& tc = list.Add("Remind Me!");
    tc.Add("Sofia Vergara");
    tc.Add("Salma Hayek");
    tc.Add("Eva Longoria");
    tc.Add("Rita Moreno");
    tc.Add("Jennifer Lopez");}

    {auto& tc = list.Add("Book an appointment");
    tc.Add("Sofia Vergara");
    tc.Add("Priyanka Chopra");
    tc.Add("Eva Mendes");
    tc.Add("Sofia Carson");
    tc.Add("Jennifer Lopez");}

    {auto& tc = list.Add("Order Books");
    tc.Add("Emma Watson");
    tc.Add("Meryl Streep");
    tc.Add("Oprah Winfrey");
    tc.Add("Reese Witherspoon");
    tc.Add("Ellen DeGeneres");}

    {auto& tc = list.Add("Download Content");
    tc.Add("Ariana Grande");
    tc.Add("Hailee Steinfeld");
    tc.Add("Anne Hathaway");
    tc.Add("Selena Gomez");
    tc.Add("Zendaya");}

    {auto& tc = list.Add("Unleash Your Creativity");
    tc.Add("Mindy Kaling");
    tc.Add("Tina Fey");
    tc.Add("Amy Poehler");
    tc.Add("Melissa McCarthy");
    tc.Add("Kristen Wiig");}

    {auto& tc = list.Add("Reserve Your Space");
    tc.Add("Beyoncé");
    tc.Add("Rihanna");
    tc.Add("Lady Gaga");
    tc.Add("Madonna");
    tc.Add("Demi Lovato");}

    {auto& tc = list.Add("Sign Up Now!");
    tc.Add("Jennifer Lawrence");
    tc.Add("Sandra Bullock");
    tc.Add("Kate Winslet");
    tc.Add("Nicole Kidman");
    tc.Add("Meryl Streep");}

    {auto& tc = list.Add("Subscribe");
    tc.Add("Ellen DeGeneres");
    tc.Add("Tyra Banks");
    tc.Add("Heidi Klum");
    tc.Add("Chrissy Teigen");
    tc.Add("Kendall Jenner");}

    {auto& tc = list.Add("Order Anything");
    tc.Add("Gwyneth Paltrow");
    tc.Add("Jessica Alba");
    tc.Add("Blake Lively");
    tc.Add("Kate Hudson");
    tc.Add("Drew Barrymore");}

    {auto& tc = list.Add("Get Our Monthly Tips Email");
    tc.Add("Gisele Bündchen");
    tc.Add("Serena Williams");
    tc.Add("Alex Morgan");
    tc.Add("Simone Biles");
    tc.Add("Megan Rapinoe");}

    {auto& tc = list.Add("Check Out The {{COMPANY}} Webstore");
    tc.Add("Kim Kardashian");
    tc.Add("Beyoncé");
    tc.Add("Kendall Jenner");
    tc.Add("Sofia Vergara");
    tc.Add("Rihanna");}
	
	return list;
}

VectorMap<String,Vector<String>>& GetTriggerUnsafeMale() {
	thread_local static VectorMap<String,Vector<String>> list;
	
	if (!list.IsEmpty()) return list;
	
	{auto& tc = list.Add("Invest");
    tc.Add("Jordan Belfort");
    tc.Add("Bernard Madoff");
    tc.Add("Elon Musk");
    tc.Add("Kevin O'Leary (aka Mr. Wonderful)");
    tc.Add("Martin Shkreli");}

    {auto& tc = list.Add("Grow");
    tc.Add("Grant Cardone");
    tc.Add("Gary Vaynerchuk");
    tc.Add("Tony Robbins");
    tc.Add("Robert Kiyosaki");
    tc.Add("Tim Ferriss");}

    {auto& tc = list.Add("Advertise");
    tc.Add("Donald Trump");
    tc.Add("Tom Cruise (in his portrayal of Jerry Maguire)");
    tc.Add("Russell Brand");
    tc.Add("Billy Mays");
    tc.Add("Billy McFarland");}

    {auto& tc = list.Add("Share Feedback");
    tc.Add("Gordon Ramsay");
    tc.Add("Simon Cowell");
    tc.Add("Steve Jobs");
    tc.Add("Piers Morgan");
    tc.Add("Simon Sinek");}

    {auto& tc = list.Add("Discuss");
    tc.Add("Howard Stern");
    tc.Add("Joe Rogan");
    tc.Add("Tucker Carlson");
    tc.Add("Bill O'Reilly");
    tc.Add("Glenn Beck");}

    {auto& tc = list.Add("View Ads");
    tc.Add("Al Pacino (in his portrayal of Ricky Roma in \"Glengarry Glen Ross\")");
    tc.Add("Matthew McConaughey (in his portrayal of Mark Hanna in \"The Wolf of Wall Street\")");
    tc.Add("Christian Bale (in his portrayal of Patrick Bateman in \"American Psycho\")");
    tc.Add("Bradley Cooper (in his portrayal of Eddie Morra in \"Limitless\")");
    tc.Add("Ben Affleck (in his portrayal of Charles Redford in \"Boiler Room\")");}

    {auto& tc = list.Add("Refer/Share");
    tc.Add("Nathan Fielder");
    tc.Add("Steve Carell (in his role as Michael Scott in \"The Office\")");
    tc.Add("Andy Samberg (in his role as Jake Peralta in \"Brooklyn Nine-Nine\")");
    tc.Add("Sacha Baron Cohen (in his role as Borat)");
    tc.Add("Larry David (in his role as himself in \"Curb Your Enthusiasm\")");}

    {auto& tc = list.Add("Get a Freebie");
    tc.Add("Charlie Sheen");
    tc.Add("Shia LaBeouf");
    tc.Add("Johnny Depp");
    tc.Add("Kanye West");
    tc.Add("Justin Bieber");}

    {auto& tc = list.Add("Remind Me!");
    tc.Add("Jason Statham");
    tc.Add("Liam Neeson");
    tc.Add("Vin Diesel");
    tc.Add("Sylvester Stallone");
    tc.Add("Bruce Willis");}

    {auto& tc = list.Add("Book an appointment");
    tc.Add("Jeremy Piven (as Ari Gold in \"Entourage\")");
    tc.Add("Alec Baldwin (as Jack Donaghy in \"30 Rock\")");
    tc.Add("Jeffrey Tambor (as George Bluth Sr. in \"Arrested Development\")");
    tc.Add("Kevin Spacey (as Frank Underwood in \"House of Cards\")");
    tc.Add("Jason Bateman (as Michael Bluth in \"Arrested Development\")");}

    {auto& tc = list.Add("Order Books");
    tc.Add("Anthony Hopkins (as Hannibal Lecter in \"The Silence of the Lambs\")");
    tc.Add("Ian McKellen (as Magneto in the \"X-Men\" franchise)");
    tc.Add("Ben Kingsley (as Don Logan in \"Sexy Beast\")");
    tc.Add("Christoph Waltz (as Hans Landa in \"Inglourious Basterds\")");
    tc.Add("Ralph Fiennes (as Voldemort in the \"Harry Potter\" franchise)");}

    {auto& tc = list.Add("Download Content");
    tc.Add("Edward Norton (as Tyler Durden in \"Fight Club\")");
    tc.Add("Leonardo DiCaprio (as Jordan Belfort in \"The Wolf of Wall Street\")");
    tc.Add("Christian Bale (as Bruce Wayne in \"The Dark Knight\")");
    tc.Add("Tom Hardy (as Bane in \"The Dark Knight Rises\")");
    tc.Add("Robert Downey Jr. (as Tony Stark in the Marvel Cinematic Universe)");}

    {auto& tc = list.Add("Unleash Your Creativity");
    tc.Add("Jim Carrey");
    tc.Add("Will Ferrell");
    tc.Add("Robin Williams");
    tc.Add("Jerry Seinfeld");
    tc.Add("Chris Rock");}

    {auto& tc = list.Add("Reserve Your Space");
    tc.Add("Samuel L. Jackson (as Jules Winnfield in \"Pulp Fiction\")");
    tc.Add("Denzel Washington (as Frank Lucas in \"American Gangster\")");
    tc.Add("Al Pacino (as Tony Montana in \"Scarface\")");
    tc.Add("Ray Liotta (as Henry Hill in \"Goodfellas\")");
    tc.Add("Johnny Depp (as James \"Whitey\" Bulger in \"Black Mass\")");}

    {auto& tc = list.Add("Sign Up Now!");
    tc.Add("Will Smith (as Chris Gardner in \"The Pursuit of Happyness\")");
    tc.Add("Tom Hanks (as Chuck Noland in \"Cast Away\")");
    tc.Add("Jake Gyllenhaal (as Louis Bloom in \"Nightcrawler\")");
    tc.Add("Brad Pitt (as Billy Beane in \"Moneyball\")");
    tc.Add("Ben Affleck (as Nick Dunne in \"Gone Girl\")");}

    {auto& tc = list.Add("Subscribe");
    tc.Add("Hugh Jackman (as Wolverine in the \"X-Men\" franchise)");
    tc.Add("Al Pacino (as Michael Corleone in \"The Godfather\")");
    tc.Add("Heath Ledger (as the Joker in \"The Dark Knight\")");
    tc.Add("Javier Bardem (as Anton Chigurh in \"No Country for Old Men\")");
    tc.Add("Daniel Day-Lewis (as Daniel Plainview in \"There Will Be Blood\")");}

    {auto& tc = list.Add("Order Anything");
    tc.Add("Robert De Niro (as Travis Bickle in \"Taxi Driver\")");
    tc.Add("Heath Ledger (as Patrick Bateman in \"American Psycho\")");
    tc.Add("Edward Norton (as Derek Vinyard in \"American History X\")");
    tc.Add("Christian Bale (in his role as the Machinist)");
    tc.Add("Jake Gyllenhaal (as Lou Bloom in \"Nightcrawler\")");}

    {auto& tc = list.Add("Get Our Monthly Tips Email");
    tc.Add("Meryl Streep (in her role as Miranda Priestly in \"The Devil Wears Prada\")");
    tc.Add("Glenn Close (as Alex Forrest in \"Fatal Attraction\")");
    tc.Add("Sharon Stone (as Catherine Tramell in \"Basic Instinct\")");
    tc.Add("Angelina Jolie (as Maleficent in the film of the same name)");
    tc.Add("Viola Davis (in her role as Annalise Keating in \"How to Get Away with Murder\")");}

    {auto& tc = list.Add("Get a {{COMPANY}} Reading List");
    tc.Add("Jeremy Irons (as Simon Gruber in \"Die Hard with a Vengeance\")");
    tc.Add("Sean Connery (as James Bond in various films)");
    tc.Add("Gene Hackman (as Lex Luthor in the \"Superman\" franchise)");
    tc.Add("Kevin Spacey (as Verbal Kint in \"The Usual Suspects\")");
    tc.Add("Samuel L. Jackson (as Mace Windu in the \"Star Wars\" prequel trilogy)");}

    {auto& tc = list.Add("Check Out The {{COMPANY}} Webstore");
    tc.Add("Jack Nicholson (as Jack Torrance in \"The Shining\")");
    tc.Add("Heath Ledger (as the Joker in \"The Dark Knight\")");
    tc.Add("Christopher Walken (as Max Zorin in \"A View to a Kill\")");
    tc.Add("Christoph Waltz (as Blofeld in \"Spectre\")");
    tc.Add("Mark Hamill (as the Joker in various animated Batman films)");}
	
	return list;
}

VectorMap<String,Vector<String>>& GetTriggerUnsafeFemale() {
	thread_local static VectorMap<String,Vector<String>> list;
	
	if (!list.IsEmpty()) return list;
	
	{auto& tc = list.Add("Invest");
    tc.Add("Martha Stewart");
    tc.Add("Elizabeth Holmes");
    tc.Add("Farrah Abraham");
    tc.Add("Tyra Banks");
    tc.Add("Sophia Amoruso");}

    {auto& tc = list.Add("Grow");
    tc.Add("Arianna Huffington");
    tc.Add("Sheryl Sandberg");
    tc.Add("Kim Kardashian");
    tc.Add("Oprah Winfrey");
    tc.Add("Gwyneth Paltrow");}

    {auto& tc = list.Add("Advertise");
    tc.Add("Kylie Jenner");
    tc.Add("Paris Hilton");
    tc.Add("Joanna Gaines");
    tc.Add("Taylor Swift");
    tc.Add("Sophia Vergara");}

    {auto& tc = list.Add("Share Feedback");
    tc.Add("Wendy Williams");
    tc.Add("Ellen DeGeneres");
    tc.Add("Chelsea Handler");
    tc.Add("Joan Rivers");
    tc.Add("Rosie O'Donnell");}

    {auto& tc = list.Add("Discuss");
    tc.Add("Anne Coulter");
    tc.Add("Samantha Bee");
    tc.Add("Megyn Kelly");
    tc.Add("Alexandria Ocasio-Cortez");
    tc.Add("Sarah Palin");}

    {auto& tc = list.Add("View Ads");
    tc.Add("Victoria Beckham");
    tc.Add("Heidi Klum");
    tc.Add("Cindy Crawford");
    tc.Add("Kate Moss");
    tc.Add("Miranda Kerr");}

    {auto& tc = list.Add("Refer/Share");
    tc.Add("Kim Kardashian");
    tc.Add("Chrissy Teigen");
    tc.Add("Gisele Bundchen");
    tc.Add("Beyonce");
    tc.Add("Ellen DeGeneres");}

    {auto& tc = list.Add("Get a Freebie");
    tc.Add("Paris Hilton");
    tc.Add("Khloe Kardashian");
    tc.Add("Rihanna");
    tc.Add("Nicki Minaj");
    tc.Add("Miley Cyrus");}

    {auto& tc = list.Add("Remind Me!");
    tc.Add("Oprah Winfrey");
    tc.Add("Martha Stewart");
    tc.Add("Bethenny Frankel");
    tc.Add("Kris Jenner");
    tc.Add("Tyra Banks");}

    {auto& tc = list.Add("Book an appointment");
    tc.Add("Martha Stewart");
    tc.Add("Martha Stewart");
    tc.Add("Rachel Ray");
    tc.Add("Ina Garten");
    tc.Add("The Pioneer Woman");}

    {auto& tc = list.Add("Order Books");
    tc.Add("J.K. Rowling");
    tc.Add("E.L. James");
    tc.Add("Danielle Steel");
    tc.Add("Nora Roberts");
    tc.Add("Suzanne Collins");}

    {auto& tc = list.Add("Download Content");
    tc.Add("Sophia Amoruso");
    tc.Add("Michelle Obama");
    tc.Add("Rachel Hollis");
    tc.Add("Mindy Kaling");
    tc.Add("Lilly Singh");}

    {auto& tc = list.Add("Unleash Your Creativity");
    tc.Add("Madonna");
    tc.Add("Lady Gaga");
    tc.Add("Katy Perry");
    tc.Add("Lizzo");
    tc.Add("Rihanna");}

    {auto& tc = list.Add("Reserve Your Space");
    tc.Add("Martha Stewart");
    tc.Add("Beyonce");
    tc.Add("Kate Middleton");
    tc.Add("Angelina Jolie");
    tc.Add("Gwyneth Paltrow");}

    {auto& tc = list.Add("Sign Up Now!");
    tc.Add("Ivanka Trump");
    tc.Add("Caroline Kennedy");
    tc.Add("Hilaria Baldwin");
    tc.Add("Sarah Jessica Parker");
    tc.Add("Reese Witherspoon");}

    {auto& tc = list.Add("Subscribe");
    tc.Add("Jenna Kutcher");
    tc.Add("Gabby Bernstein");
    tc.Add("Maria Sharapova");
    tc.Add("Blake Lively");
    tc.Add("Amal Clooney");}

    {auto& tc = list.Add("Order Anything");
    tc.Add("Martha Stewart");
    tc.Add("Rachel Ray");
    tc.Add("Ina Garten");
    tc.Add("The Pioneer Woman");
    tc.Add("Julia Child");}

    {auto& tc = list.Add("Get Our Monthly Tips Email");
    tc.Add("Marie Forleo");
    tc.Add("Cheryl Strayed");
    tc.Add("Brene Brown");
    tc.Add("Mel Robbins");
    tc.Add("Caroline Myss");}

    {auto& tc = list.Add("Check Out The {{COMPANY}} Webstore");
    tc.Add("Kim Kardashian");
    tc.Add("Kylie Jenner");
    tc.Add("Rihanna");
    tc.Add("Jessica Alba");
    tc.Add("Jennifer Lopez");}

    {auto& tc = list.Add("Get a {{COMPANY}} Reading List");
    tc.Add("Oprah Winfrey");
    tc.Add("Reese Witherspoon");
    tc.Add("Emma Watson");
    tc.Add("Reese Witherspoon");
    tc.Add("Lupita Nyong'o");}
	
	return list;
}

VectorMap<String,Vector<String>>& GetTriggerSafe(bool gender) {
	if (!gender)
		return GetTriggerSafeMale();
	else
		return GetTriggerSafeFemale();
}

VectorMap<String,Vector<String>>& GetTriggerUnsafe(bool gender) {
	if (!gender)
		return GetTriggerUnsafeMale();
	else
		return GetTriggerUnsafeFemale();
}

VectorMap<String,Vector<String>>& GetPersuasiveTriggers(bool unsafe, bool gender) {
	if (!unsafe)
		return GetTriggerSafe(gender);
	else
		return GetTriggerUnsafe(gender);
}



END_TEXTLIB_NAMESPACE
