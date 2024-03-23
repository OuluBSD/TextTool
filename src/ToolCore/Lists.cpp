#include "ToolCore.h"
#include "ProtectedCommon.h"


BEGIN_TEXTLIB_NAMESPACE


#if 0
VectorMap<String,Vector<String>>& GetTypecastSingers() {
	thread_local static VectorMap<String,Vector<String>> list;
	if (!list.IsEmpty()) return list;
	
	{auto& tc = list.Add("Heartbroken/lovesick");
	tc.Add("Adele");
	tc.Add("Sam Smith");
	tc.Add("Ed Sheeran");
	tc.Add("Patsy Cline");
	tc.Add("Amy Winehouse");
	tc.Add("Coldplay");
	tc.Add("Sinead O'Connor");}

	{auto& tc = list.Add("Rebel/anti-establishment");
	tc.Add("The Sex Pistols");
	tc.Add("Public Enemy");
	tc.Add("Sex Pistols");
	tc.Add("Dead Kennedys");
	tc.Add("Black Flag");
	tc.Add("The Clash");
	tc.Add("Rage Against the Machine");
	tc.Add("Nirvana");
	tc.Add("Green Day");}

	{auto& tc = list.Add("Political activist");
	tc.Add("Bob Marley");
	tc.Add("Rage Against the Machine");
	tc.Add("Bob Dylan");
	tc.Add("Public Enemy");
	tc.Add("Joan Baez");
	tc.Add("John Lennon");
	tc.Add("Neil Young");
	tc.Add("Dixie Chicks");
	tc.Add("Fela Kuti");}

	{auto& tc = list.Add("Social justice advocate");
	tc.Add("Bob Marley");
	tc.Add("Public Enemy");
	tc.Add("Beyonce");
	tc.Add("Marvin Gaye");
	tc.Add("Lauryn Hill");
	tc.Add("Nina Simone");
	tc.Add("Billie Holiday");
	tc.Add("Sam Cooke");
	tc.Add("Tracy Chapman");}

	{auto& tc = list.Add("Party/club");
	tc.Add("The Black Eyed Peas");
	tc.Add("LMFAO");
	tc.Add("Calvin Harris");
	tc.Add("Rihanna");
	tc.Add("Bruno Mars");
	tc.Add("Pitbull");
	tc.Add("Jason Derulo");
	tc.Add("Flo Rida");
	tc.Add("DNCE");}

	{auto& tc = list.Add("Hopeful/dreamer");
	tc.Add("John Lennon");
	tc.Add("Bob Marley");
	tc.Add("Michael Jackson");
	tc.Add("Coldplay");
	tc.Add("U2");
	tc.Add("Imagine Dragons");
	tc.Add("Miley Cyrus");
	tc.Add("Beyonce");}

	{auto& tc = list.Add("Confident/empowered");
	tc.Add("Beyonce");
	tc.Add("Whitney Houston");
	tc.Add("Destiny's Child");
	tc.Add("Queen Latifah");
	tc.Add("Demi Lovato");
	tc.Add("Lizzo");
	tc.Add("Cardi B");
	tc.Add("Janelle Monae");}

	{auto& tc = list.Add("Vulnerable/raw");
	tc.Add("Billie Eilish");
	tc.Add("Fiona Apple");
	tc.Add("Alanis Morissette");
	tc.Add("Elliott Smith");
	tc.Add("Adele");
	tc.Add("Brandi Carlile");
	tc.Add("Lana Del Rey");
	tc.Add("Sufjan Stevens");
	tc.Add("Lorde");}

	{auto& tc = list.Add("Romantic/love-driven");
	tc.Add("Adele");
	tc.Add("Ed Sheeran");
	tc.Add("John Legend");
	tc.Add("Bruno Mars");
	tc.Add("Sam Smith");
	tc.Add("The Weeknd");
	tc.Add("Mariah Carey");
	tc.Add("Hozier");}

	{auto& tc = list.Add("Failure/loser");
	tc.Add("Beck");
	tc.Add("The Smiths");
	tc.Add("Morrissey");
	tc.Add("The Replacements");
	tc.Add("Green Day (early years)");
	tc.Add("Kanye West (808s and Heartbreak)");
	tc.Add("Bon Iver");
	tc.Add("The National");
	tc.Add("The Cure");}

	{auto& tc = list.Add("Spiritual/faithful");
	tc.Add("Bob Marley");
	tc.Add("George Harrison");
	tc.Add("Madonna");
	tc.Add("Sufjan Stevens");
	tc.Add("Cat Stevens");
	tc.Add("Leonard Cohen");
	tc.Add("Mavis Staples");
	tc.Add("Enya");
	tc.Add("Dave Matthews Band");}

	{auto& tc = list.Add("Passionate/determined");
	tc.Add("Beyonce");
	tc.Add("Eminem");
	tc.Add("Kanye West");
	tc.Add("Pink");
	tc.Add("Bruce Springsteen");
	tc.Add("Shakira");
	tc.Add("Aerosmith");}

	{auto& tc = list.Add("Reflective/self-reflective");
	tc.Add("Lana Del Rey");
	tc.Add("Bon Iver");
	tc.Add("Fleet Foxes");
	tc.Add("Sufjan Stevens");
	tc.Add("Elliott Smith");
	tc.Add("Nick Drake");
	tc.Add("Phoebe Bridgers");
	tc.Add("Radiohead");
	tc.Add("Joni Mitchell");}

	{auto& tc = list.Add("Witty/sarcastic");
	tc.Add("Weird Al Yankovic");
	tc.Add("Flight of the Conchords");
	tc.Add("Tenacious D");
	tc.Add("The Lonely Island");
	tc.Add("Bo Burnham");
	tc.Add("Stephen Lynch");
	tc.Add("Tim Minchin");
	tc.Add("Randy Newman");
	tc.Add("Weird Al Yankovic");}

	{auto& tc = list.Add("Melancholic/sad");
	tc.Add("Billie Eilish");
	tc.Add("Lana Del Rey");
	tc.Add("Elliot Smith");
	tc.Add("Bon Iver");
	tc.Add("The Smiths");
	tc.Add("Sufjan Stevens");
	tc.Add("Adele");
	tc.Add("Sam Smith");
	tc.Add("Hozier");}

	{auto& tc = list.Add("Humble/down-to-earth");
	tc.Add("Johnny Cash");
	tc.Add("Bob Dylan");
	tc.Add("Mumford & Sons");
	tc.Add("The Lumineers");
	tc.Add("Jason Isbell");
	tc.Add("The Avett Brothers");
	tc.Add("Fleet Foxes");
	tc.Add("Ray LaMontagne");
	tc.Add("Iron & Wine");}

	{auto& tc = list.Add("Charismatic/charming");
	tc.Add("Frank Sinatra");
	tc.Add("Freddie Mercury");
	tc.Add("Prince");
	tc.Add("Michael Jackson");
	tc.Add("Elvis Presley");
	tc.Add("Bruno Mars");
	tc.Add("Justin Timberlake");
	tc.Add("Beyonce");
	tc.Add("Ariana Grande");}

	{auto& tc = list.Add("Resilient/overcoming adversity");
	tc.Add("Demi Lovato");
	tc.Add("Eminem");
	tc.Add("Lizzo");
	tc.Add("Pink");
	tc.Add("Kelly Clarkson");
	tc.Add("Christina Aguilera");
	tc.Add("Mariah Carey");
	tc.Add("Janelle Monae");}

	{auto& tc = list.Add("Carefree/joyful");
	tc.Add("Pharrell Williams");
	tc.Add("Bruno Mars");
	tc.Add("Walk the Moon");
	tc.Add("Carly Rae Jepsen");
	tc.Add("Fitz and the Tantrums");
	tc.Add("ABBA");
	tc.Add("B-52s");
	tc.Add("Prince");}

	{auto& tc = list.Add("Dark/mysterious");
	tc.Add("The Weeknd");
	tc.Add("Lana Del Rey");
	tc.Add("Nick Cave & the Bad Seeds");
	tc.Add("Mazzy Star");
	tc.Add("David Bowie");
	tc.Add("Portishead");
	tc.Add("Depeche Mode");
	tc.Add("Nine Inch Nails");
	tc.Add("Radiohead (early years)");}

	{auto& tc = list.Add("Comical/humorous");
	tc.Add("Weird Al Yankovic");
	tc.Add("Flight of the Conchords");
	tc.Add("The Lonely Island");
	tc.Add("Bo Burnham");
	tc.Add("Tenacious D");
	tc.Add("Stephen Lynch");
	tc.Add("Tim Minchin");
	tc.Add("Randy Newman");
	tc.Add("Adam Sandler");}

	{auto& tc = list.Add("Controversial/provocative");
	tc.Add("Marilyn Manson");
	tc.Add("Eminem");
	tc.Add("Madonna");
	tc.Add("Nine Inch Nails");
	tc.Add("Rage Against the Machine");
	tc.Add("Dead Kennedys");
	tc.Add("Public Enemy");
	tc.Add("Lizzo");
	tc.Add("Cardi B");}

	{auto& tc = list.Add("Nostalgic/sentimental");
	tc.Add("Fleetwood Mac");
	tc.Add("The Beach Boys");
	tc.Add("Bruce Springsteen");
	tc.Add("Queen");
	tc.Add("Billy Joel");
	tc.Add("Joni Mitchell");
	tc.Add("Elton John");
	tc.Add("ABBA");
	tc.Add("Whitney Houston");}

	{auto& tc = list.Add("Wise/philosophical");
	tc.Add("Bob Dylan");
	tc.Add("Leonard Cohen");
	tc.Add("Joni Mitchell");
	tc.Add("Neil Young");
	tc.Add("James Taylor");
	tc.Add("Cat Stevens");
	tc.Add("Simon & Garfunkel");
	tc.Add("Van Morrison");
	tc.Add("Pink Floyd");}

	{auto& tc = list.Add("Angry/outspoken");
	tc.Add("Rage Against the Machine");
	tc.Add("Public Enemy");
	tc.Add("N.W.A");
	tc.Add("Dead Kennedys");
	tc.Add("Black Flag");
	tc.Add("The Clash");
	tc.Add("Bikini Kill");
	tc.Add("Eminem");
	tc.Add("Bob Dylan (protest components)");}

	{auto& tc = list.Add("Calm/peaceful");
	tc.Add("Jack Johnson");
	tc.Add("Norah Jones");
	tc.Add("Bon Iver");
	tc.Add("Iron & Wine");
	tc.Add("Nick Drake");
	tc.Add("Elliott Smith");
	tc.Add("Tallest Man on Earth");
	tc.Add("José González");
	tc.Add("John Mayer");}
	
	{auto& tc = list.Add("Confident/self-assured");
	tc.Add("Beyoncé");
	tc.Add("Eminem");
	tc.Add("Kanye West");
	tc.Add("Drake");}
	
	{auto& tc = list.Add("Self-destructive/self-sabotaging");
	tc.Add("Amy Winehouse");
	tc.Add("Kurt Cobain");
	tc.Add("Jimi Hendrix");
	tc.Add("Johnny Cash");
	tc.Add("Janis Joplin");
	tc.Add("Jeff Buckley");
	tc.Add("Gram Parsons");
	tc.Add("Elliott Smith");
	tc.Add("Sid Vicious");}

	{auto& tc = list.Add("Hopeful/optimistic");
	tc.Add("Coldplay");
	tc.Add("The Beatles");
	tc.Add("Bob Marley");
	tc.Add("Bruce Springsteen");
	tc.Add("U2");
	tc.Add("The Lumineers");
	tc.Add("Imagine Dragons");
	tc.Add("Miley Cyrus");}

	{auto& tc = list.Add("Fearful/anxious");
	tc.Add("Lana Del Rey");
	tc.Add("Linkin Park (early years)");
	tc.Add("Sia");
	tc.Add("Hole");
	tc.Add("Fiona Apple");
	tc.Add("Radiohead (early years)");
	tc.Add("Arcade Fire");
	tc.Add("Marilyn Manson");}

	{auto& tc = list.Add("Eccentric/quirky");
	tc.Add("Björk");
	tc.Add("David Bowie");
	tc.Add("Regina Spektor");
	tc.Add("Florence + The Machine");
	tc.Add("Talking Heads");
	tc.Add("St. Vincent");
	tc.Add("Kate Bush");
	tc.Add("Tom Waits");
	tc.Add("Alt-J");}

	{auto& tc = list.Add("Sensitive/emotional");
	tc.Add("Bon Iver");
	tc.Add("Adele");
	tc.Add("Radiohead");
	tc.Add("The National");
	tc.Add("Iron & Wine");
	tc.Add("Fleet Foxes");
	tc.Add("Nick Drake");
	tc.Add("Coldplay");
	tc.Add("Lana Del Rey");}

	{auto& tc = list.Add("Bitter/resentful");
	tc.Add("Alanis Morissette");
	tc.Add("Fleetwood Mac (Rumours album)");
	tc.Add("Kanye West (My Beautiful Dark Twisted Fantasy)");
	tc.Add("Lorde");
	tc.Add("Fiona Apple");
	tc.Add("Kelly Clarkson");
	tc.Add("Adele");
	tc.Add("Billie Eilish");}

	{auto& tc = list.Add("Unique/nonconformist");
	tc.Add("David Bowie");
	tc.Add("Madonna");
	tc.Add("Prince");
	tc.Add("Björk");
	tc.Add("Prince");
	tc.Add("Kate Bush");
	tc.Add("Peter Gabriel");
	tc.Add("Queen");}

	{auto& tc = list.Add("Free-spirited/nonconformist");
	tc.Add("Bob Dylan");
	tc.Add("Janis Joplin");
	tc.Add("Joni Mitchell");
	tc.Add("Jimi Hendrix");
	tc.Add("The Doors");
	tc.Add("The Rolling Stones");
	tc.Add("Pink Floyd (early years)");
	tc.Add("Patti Smith");
	tc.Add("Fleetwood Mac (early years)");}

	{auto& tc = list.Add("Sultry/seductive");
	tc.Add("Lana Del Rey");
	tc.Add("The Weeknd");
	tc.Add("Marvin Gaye");
	tc.Add("Beyonce");
	tc.Add("Prince");
	tc.Add("Sade");
	tc.Add("Tove Lo");
	tc.Add("Hozier");
	tc.Add("The xx");}

	{auto& tc = list.Add("Inspirational/motivational");
	tc.Add("Michael Jackson");
	tc.Add("Beyonce");
	tc.Add("Eminem");
	tc.Add("Queen");
	tc.Add("Madonna");
	tc.Add("Coldplay");
	tc.Add("U2");
	tc.Add("Kendrick Lamar");
	tc.Add("Imagine Dragons");}

	{auto& tc = list.Add("Authentic/real");
	tc.Add("Johnny Cash");
	tc.Add("Bob Dylan");
	tc.Add("Bruce Springsteen");
	tc.Add("Joni Mitchell");
	tc.Add("Neil Young");
	tc.Add("Leonard Cohen");
	tc.Add("Cat Stevens");
	tc.Add("Sufjan Stevens");
	tc.Add("Tracy Chapman");}

	{auto& tc = list.Add("Mysterious/enigmatic");
	tc.Add("David Bowie");
	tc.Add("Prince");
	tc.Add("Lana Del Rey");
	tc.Add("Radiohead");
	tc.Add("Leonard Cohen");
	tc.Add("Fleetwood Mac");
	tc.Add("Nick Cave & the Bad Seeds");
	tc.Add("Kate Bush");
	tc.Add("Björk");}

	{auto& tc = list.Add("Carefree/bohemian");
	tc.Add("Fleetwood Mac (Rumours album)");
	tc.Add("Led Zeppelin");
	tc.Add("Janis Joplin");
	tc.Add("Crosby, Stills, & Nash");
	tc.Add("The Doors");
	tc.Add("The Grateful Dead");
	tc.Add("Jefferson Airplane");
	tc.Add("Bob Dylan (1960s era)");
	tc.Add("Donovan");}

	{auto& tc = list.Add("Street-smart/tough");
	tc.Add("Eminem");
	tc.Add("Tupac Shakur");
	tc.Add("The Notorious B.I.G.");
	tc.Add("Queen Latifah");
	tc.Add("N.W.A");
	tc.Add("Public Enemy");
	tc.Add("Jay-Z");
	tc.Add("Run-DMC");
	tc.Add("LL Cool J");}

	{auto& tc = list.Add("Romantic/idealistic");
	tc.Add("John Mayer");
	tc.Add("Ed Sheeran");
	tc.Add("Bruno Mars");
	tc.Add("Mariah Carey");
	tc.Add("Michael Bublé");
	tc.Add("Jason Mraz");
	tc.Add("Shawn Mendes");
	tc.Add("Stevie Wonder");}

	{auto& tc = list.Add("Nurturing/motherly");
	tc.Add("Beyonce");
	tc.Add("Aretha Franklin");
	tc.Add("Dolly Parton");
	tc.Add("Mariah Carey");
	tc.Add("Alicia Keys");
	tc.Add("Adele");
	tc.Add("Sade");
	tc.Add("Barbra Streisand");}

	{auto& tc = list.Add("Dark/tormented");
	tc.Add("Kurt Cobain (Nirvana)");
	tc.Add("Amy Winehouse");
	tc.Add("Chester Bennington (Linkin Park)");
	tc.Add("Billie Eilish");
	tc.Add("Marilyn Manson");
	tc.Add("Nine Inch Nails");
	tc.Add("Joy Division");
	tc.Add("David Bowie (Berlin era)");
	tc.Add("The Cure");}

	{auto& tc = list.Add("Remorseful/regretful");
	tc.Add("Adele");
	tc.Add("Sam Smith");
	tc.Add("Beyonce");
	tc.Add("Bon Iver");
	tc.Add("Lana Del Rey");
	tc.Add("Billie Eilish");
	tc.Add("Ed Sheeran");
	tc.Add("Elliott Smith");
	tc.Add("Chris Cornell (Soundgarden, Audioslave)");}

	{auto& tc = list.Add("Bold/brave");
	tc.Add("Freddie Mercury (Queen)");
	tc.Add("Beyonce");
	tc.Add("Eminem");
	tc.Add("Janis Joplin");
	tc.Add("Madonna");
	tc.Add("Kendrick Lamar");
	tc.Add("Pink");
	tc.Add("Frank Sinatra");}

	{auto& tc = list.Add("Outcast/rebel");
	tc.Add("The Clash");
	tc.Add("Nirvana");
	tc.Add("Rage Against the Machine");
	tc.Add("The Sex Pistols");
	tc.Add("Green Day");
	tc.Add("Gary Clark Jr.");
	tc.Add("Greta Van Fleet");
	tc.Add("Joan Jett");
	tc.Add("Bikini Kill");}

	{auto& tc = list.Add("Lost/disconnected");
	tc.Add("Radiohead");
	tc.Add("Lana Del Rey");
	tc.Add("The Weeknd");
	tc.Add("Nirvana");
	tc.Add("The Smiths");
	tc.Add("Pink Floyd (Dark Side of the Moon era)");
	tc.Add("David Bowie (Berlin era)");
	tc.Add("Morrissey");
	tc.Add("The Doors");}

	{auto& tc = list.Add("Tough/badass");
	tc.Add("AC/DC");
	tc.Add("Guns N' Roses");
	tc.Add("Metallica");
	tc.Add("Black Sabbath");
	tc.Add("Mötley Crüe");
	tc.Add("Joan Jett");
	tc.Add("Led Zeppelin");
	tc.Add("The Rolling Stones");
	tc.Add("Limp Bizkit");}

	{auto& tc = list.Add("Sincere/genuine");
	tc.Add("Bruce Springsteen");
	tc.Add("Bob Marley");
	tc.Add("Fleetwood Mac (Rumours album)");
	tc.Add("Tracy Chapman");
	tc.Add("Cat Stevens");
	tc.Add("Simon & Garfunkel");
	tc.Add("Ray Charles");
	tc.Add("Sam Cooke");
	tc.Add("Bill Withers");}

	{auto& tc = list.Add("Honest/vulnerable");
	tc.Add("Adele");
	tc.Add("Ed Sheeran");
	tc.Add("Billie Eilish");
	tc.Add("Lorde");
	tc.Add("Hozier");
	tc.Add("Lana Del Rey");
	tc.Add("James Bay");
	tc.Add("Shawn Mendes");}

	{auto& tc = list.Add("Innocent/naive");
	tc.Add("Ed Sheeran");
	tc.Add("Billie Eilish");
	tc.Add("Shawn Mendes");
	tc.Add("Dodie Clark");
	tc.Add("The Beach Boys");
	tc.Add("Stevie Wonder");
	tc.Add("The Supremes");
	tc.Add("ABBA");}

	{auto& tc = list.Add("Bold/risk-taking");
	tc.Add("David Bowie");
	tc.Add("Madonna");
	tc.Add("Prince");
	tc.Add("Beyonce");
	tc.Add("Pink Floyd (early years)");
	tc.Add("Patti Smith");
	tc.Add("The Rolling Stones");
	tc.Add("Queen");
	}
	return list;
}

VectorMap<String,Vector<String>>& GetTypecastRappers() {
	thread_local static VectorMap<String,Vector<String>> list;
	if (!list.IsEmpty()) return list;
	

	{auto& tc = list.Add("Heartbroken/lovesick");
	tc.Add("Drake");
	tc.Add("XXXTENTACION");
	tc.Add("The Weeknd");
	tc.Add("Frank Ocean");
	tc.Add("Kid Cudi");}

	{auto& tc = list.Add("Rebel/anti-establishment");
	tc.Add("N.W.A");
	tc.Add("Public Enemy");
	tc.Add("Rage Against The Machine");
	tc.Add("Dead Prez");
	tc.Add("Immortal Technique");}

	{auto& tc = list.Add("Political activist");
	tc.Add("Killer Mike");
	tc.Add("Common");
	tc.Add("Talib Kweli");
	tc.Add("KRS-One");
	tc.Add("Lupe Fiasco");}

	{auto& tc = list.Add("Social justice advocate");
	tc.Add("Kendrick Lamar");
	tc.Add("J. Cole");
	tc.Add("Chance the Rapper");
	tc.Add("Noname");
	tc.Add("Vic Mensa");}

	{auto& tc = list.Add("Party/club");
	tc.Add("Cardi B");
	tc.Add("Migos");
	tc.Add("A$AP Rocky");
	tc.Add("Lil Jon");
	tc.Add("Flo Rida");}

	{auto& tc = list.Add("Hopeful/dreamer");
	tc.Add("Logic");
	tc.Add("Kanye West");
	tc.Add("Mac Miller");
	tc.Add("Wiz Khalifa");
	tc.Add("Saba");}

	{auto& tc = list.Add("Confident/empowered");
	tc.Add("Nicki Minaj");
	tc.Add("Lil Kim");
	tc.Add("Missy Elliott");
	tc.Add("Queen Latifah");
	tc.Add("Remy Ma");}

	{auto& tc = list.Add("Vulnerable/raw");
	tc.Add("Eminem");
	tc.Add("Tyler, The Creator");
	tc.Add("Kid Cudi");
	tc.Add("Danny Brown");
	tc.Add("Earl Sweatshirt");}

	{auto& tc = list.Add("Romantic/love-driven");
	tc.Add("Common");
	tc.Add("J. Cole");
	tc.Add("Chance the Rapper");
	tc.Add("Bryson Tiller");
	tc.Add("Khalid");}

	{auto& tc = list.Add("Failure/loser");
	tc.Add("50 Cent");
	tc.Add("Jay Rock");
	tc.Add("Lil Peep");
	tc.Add("NF");
	tc.Add("Future");}

	{auto& tc = list.Add("Spiritual/faithful");
	tc.Add("Kanye West");
	tc.Add("Chance the Rapper");
	tc.Add("Isaiah Rashad");
	tc.Add("Mick Jenkins");
	tc.Add("Anderson .Paak");}

	{auto& tc = list.Add("Passionate/determined");
	tc.Add("Eminem");
	tc.Add("Kendrick Lamar");
	tc.Add("T.I.");
	tc.Add("Meek Mill");
	tc.Add("Joey Bada$$");}

	{auto& tc = list.Add("Reflective/self-reflective");
	tc.Add("J. Cole");
	tc.Add("Ab-Soul");
	tc.Add("Joey Bada$$");
	tc.Add("Logic");
	tc.Add("Chance the Rapper");}

	{auto& tc = list.Add("Witty/sarcastic");
	tc.Add("Eminem");
	tc.Add("Danny Brown");
	tc.Add("Childish Gambino");
	tc.Add("Action Bronson");
	tc.Add("Lil Dicky");}

	{auto& tc = list.Add("Melancholic/sad");
	tc.Add("XXXTENTACION");
	tc.Add("Mac Miller");
	tc.Add("Post Malone");
	tc.Add("Juice WRLD");
	tc.Add("G-Eazy");}

	{auto& tc = list.Add("Humble/down-to-earth");
	tc.Add("Nas");
	tc.Add("J. Cole");
	tc.Add("YG");
	tc.Add("Nipsey Hussle");
	tc.Add("Chance the Rapper");}

	{auto& tc = list.Add("Charismatic/charming");
	tc.Add("Snoop Dogg");
	tc.Add("Lil Wayne");
	tc.Add("Kanye West");
	tc.Add("Drake");
	tc.Add("A$AP Rocky");}

	{auto& tc = list.Add("Resilient/overcoming adversity");
	tc.Add("50 Cent");
	tc.Add("Tupac Shakur");
	tc.Add("DMX");
	tc.Add("Eminem");
	tc.Add("The Notorious B.I.G.");}

	{auto& tc = list.Add("Carefree/joyful");
	tc.Add("Chance the Rapper");
	tc.Add("Lil Yachty");
	tc.Add("Lil Uzi Vert");
	tc.Add("MadeinTYO");
	tc.Add("A-Trak");}

	{auto& tc = list.Add("Dark/mysterious");
	tc.Add("MF DOOM");
	tc.Add("Earl Sweatshirt");
	tc.Add("Tyler, The Creator");
	tc.Add("Denzel Curry");
	tc.Add("Flatbush Zombies");}

	{auto& tc = list.Add("Comical/humorous");
	tc.Add("Lil Dicky");
	tc.Add("Action Bronson");
	tc.Add("Childish Gambino");
	tc.Add("Riff Raff");
	tc.Add("Andy Mineo");}

	{auto& tc = list.Add("Controversial/provocative");
	tc.Add("Eminem");
	tc.Add("Tyler, The Creator");
	tc.Add("XXXTENTACION");
	tc.Add("6ix9ine");
	tc.Add("N.W.A");}

	{auto& tc = list.Add("Nostalgic/sentimental");
	tc.Add("Kanye West");
	tc.Add("J. Cole");
	tc.Add("Joey Bada$$");
	tc.Add("Mac Miller");
	tc.Add("Logic");}

	{auto& tc = list.Add("Wise/philosophical");
	tc.Add("Kendrick Lamar");
	tc.Add("J. Cole");
	tc.Add("Lupe Fiasco");
	tc.Add("Nas");
	tc.Add("Jay-Z");}

	{auto& tc = list.Add("Angry/outspoken");
	tc.Add("Immortal Technique");
	tc.Add("Rage Against The Machine");
	tc.Add("DMX");
	tc.Add("Cardi B");
	tc.Add("Ice Cube");}

	{auto& tc = list.Add("Calm/peaceful");
	tc.Add("Joey Bada$$");
	tc.Add("Vince Staples");
	tc.Add("Mick Jenkins");
	tc.Add("SZA");
	tc.Add("Noname");}

	{auto& tc = list.Add("Confident/self-assured");
	tc.Add("Cardi B");
	tc.Add("Megan Thee Stallion");
	tc.Add("Nicki Minaj");
	tc.Add("DaBaby");
	tc.Add("Future");}

	{auto& tc = list.Add("Self-destructive/self-sabotaging");
	tc.Add("Mac Miller");
	tc.Add("Juice WRLD");
	tc.Add("Kid Cudi");
	tc.Add("Eminem");
	tc.Add("DMX");}

	{auto& tc = list.Add("Hopeful/optimistic");
	tc.Add("Logic");
	tc.Add("Chance the Rapper");
	tc.Add("Mac Miller");
	tc.Add("Wiz Khalifa");
	tc.Add("Outkast");}

	{auto& tc = list.Add("Fearful/anxious");
	tc.Add("XXXTENTACION");
	tc.Add("Kendrick Lamar");
	tc.Add("Earl Sweatshirt");
	tc.Add("NF");
	tc.Add("Kid Cudi");}

	{auto& tc = list.Add("Eccentric/quirky");
	tc.Add("Lil Uzi Vert");
	tc.Add("Young Thug");
	tc.Add("Lil Yachty");
	tc.Add("Tyler, The Creator");
	tc.Add("Playboi Carti");}

	{auto& tc = list.Add("Sensitive/emotional");
	tc.Add("Eminem");
	tc.Add("Kanye West");
	tc.Add("Kid Cudi");
	tc.Add("Mac Miller");
	tc.Add("Lil Peep");}

	{auto& tc = list.Add("Bitter/resentful");
	tc.Add("Eminem");
	tc.Add("50 Cent");
	tc.Add("Joe Budden");
	tc.Add("Dr. Dre");
	tc.Add("Tupac Shakur");}

	{auto& tc = list.Add("Unique/nonconformist");
	tc.Add("A$AP Rocky");
	tc.Add("Tyler, The Creator");
	tc.Add("André 3000");
	tc.Add("Kanye West");
	tc.Add("Lil Wayne");}

	{auto& tc = list.Add("Free-spirited/nonconformist");
	tc.Add("Chance the Rapper");
	tc.Add("A$AP Rocky");
	tc.Add("Kid Cudi");
	tc.Add("Travis Scott");
	tc.Add("Smino");}

	{auto& tc = list.Add("Sultry/seductive");
	tc.Add("Cardi B");
	tc.Add("Megan Thee Stallion");
	tc.Add("Nicki Minaj");
	tc.Add("Ty Dolla $ign");
	tc.Add("SZA");}

	{auto& tc = list.Add("Inspirational/motivational");
	tc.Add("Eminem");
	tc.Add("Kanye West");
	tc.Add("J. Cole");
	tc.Add("Kendrick Lamar");
	tc.Add("Logic");}

	{auto& tc = list.Add("Authentic/real");
	tc.Add("Tupac Shakur");
	tc.Add("DMX");
	tc.Add("Eminem");
	tc.Add("The Notorious B.I.G.");
	tc.Add("Nas");}

	{auto& tc = list.Add("Mysterious/enigmatic");
	tc.Add("MF DOOM");
	tc.Add("Earl Sweatshirt");
	tc.Add("Tyler, The Creator");
	tc.Add("Kendrick Lamar");
	tc.Add("Denzel Curry");}

	{auto& tc = list.Add("Carefree/bohemian");
	tc.Add("Chance the Rapper");
	tc.Add("Kid Cudi");
	tc.Add("Tyler, The Creator");
	tc.Add("Isaiah Rashad");
	tc.Add("A$AP Rocky");}

	{auto& tc = list.Add("Street-smart/tough");
	tc.Add("Tupac Shakur");
	tc.Add("DMX");
	tc.Add("The Notorious B.I.G.");
	tc.Add("Eminem");
	tc.Add("Nas");}

	{auto& tc = list.Add("Romantic/idealistic");
	tc.Add("Common");
	tc.Add("J. Cole");
	tc.Add("Chance the Rapper");
	tc.Add("Bryson Tiller");
	tc.Add("Khalid");}

	{auto& tc = list.Add("Nurturing/motherly");
	tc.Add("Cardi B");
	tc.Add("Nicki Minaj");
	tc.Add("Missy Elliott");
	tc.Add("Queen Latifah");
	tc.Add("Lauryn Hill");}

	{auto& tc = list.Add("Dark/tormented");
	tc.Add("Tyler, The Creator");
	tc.Add("Earl Sweatshirt");
	tc.Add("MF DOOM");
	tc.Add("XXXTENTACION");
	tc.Add("Lil Peep");}

	{auto& tc = list.Add("Remorseful/regretful");
	tc.Add("Kendrick Lamar");
	tc.Add("Kid Cudi");
	tc.Add("Logic");
	tc.Add("Mac Miller");
	tc.Add("Jhené Aiko");}

	{auto& tc = list.Add("Bold/brave");
	tc.Add("Eminem");
	tc.Add("Kendrick Lamar");
	tc.Add("J. Cole");
	tc.Add("Kanye West");
	tc.Add("Jay-Z");}

	{auto& tc = list.Add("Outcast/rebel");
	tc.Add("Tyler, The Creator");
	tc.Add("MF DOOM");
	tc.Add("Kid Cudi");
	tc.Add("YG");
	tc.Add("XXXTENTACION");}

	{auto& tc = list.Add("Lost/disconnected");
	tc.Add("Kanye West");
	tc.Add("Kid Cudi");
	tc.Add("Earl Sweatshirt");
	tc.Add("Mac Miller");
	tc.Add("Travis Scott");}

	{auto& tc = list.Add("Tough/badass");
	tc.Add("Cardi B");
	tc.Add("Megan Thee Stallion");
	tc.Add("DaBaby");
	tc.Add("50 Cent");
	tc.Add("DMX");}

	{auto& tc = list.Add("Sincere/genuine");
	tc.Add("J. Cole");
	tc.Add("Logic");
	tc.Add("Kendrick Lamar");
	tc.Add("Chance the Rapper");
	tc.Add("Mac Miller");}

	{auto& tc = list.Add("Honest/vulnerable");
	tc.Add("Eminem");
	tc.Add("Kanye West");
	tc.Add("J. Cole");
	tc.Add("Kid Cudi");
	tc.Add("Anderson .Paak");}

	{auto& tc = list.Add("Innocent/naive");
	tc.Add("Chance the Rapper");
	tc.Add("Lil Yachty");
	tc.Add("Ski Mask the Slump God");
	tc.Add("Lil Baby");
	tc.Add("Sheck Wes");}

	{auto& tc = list.Add("Bold/risk-taking");
	tc.Add("Kanye West");
	tc.Add("Kid Cudi");
	tc.Add("The Notorious B.I.G.");
	tc.Add("Eminem");
	tc.Add("Travis Scott");}
	
	ASSERT(list.GetCount() == GetTypecastSingers().GetCount());
	return list;
}
#endif

VectorMap<String,Vector<String>>& GetTypecastSingersFemale() {
	thread_local static VectorMap<String,Vector<String>> list;
	if (!list.IsEmpty()) return list;
	
	{auto& tc = list.Add("Heartbroken/lovesick");
	tc.Add("Adele");
	tc.Add("Sam Smith");
	tc.Add("Beyoncé");
	tc.Add("Selena Gomez");
	tc.Add("Halsey");}

	{auto& tc = list.Add("Rebel/anti-establishment");
	tc.Add("Billie Eilish");
	tc.Add("Demi Lovato");
	tc.Add("Miley Cyrus");
	tc.Add("Alanis Morissette");
	tc.Add("Gwen Stefani");}

	{auto& tc = list.Add("Political activist");
	tc.Add("Joan Baez");
	tc.Add("Nina Simone");
	tc.Add("Ani DiFranco");
	tc.Add("Melissa Etheridge");
	tc.Add("Patti Smith");}

	{auto& tc = list.Add("Social justice advocate");
	tc.Add("Alicia Keys");
	tc.Add("Janelle Monáe");
	tc.Add("Lauryn Hill");
	tc.Add("India.Arie");
	tc.Add("Tracy Chapman");}

	{auto& tc = list.Add("Party/club");
	tc.Add("Rihanna");
	tc.Add("Cardi B");
	tc.Add("Jennifer Lopez");
	tc.Add("Shakira");
	tc.Add("Nicki Minaj");}

	{auto& tc = list.Add("Hopeful/dreamer");
	tc.Add("Demi Lovato");
	tc.Add("Selena Gomez");
	tc.Add("Alessia Cara");}

	{auto& tc = list.Add("Confident/empowered");
	tc.Add("Beyoncé");
	tc.Add("Ariana Grande");
	tc.Add("Christina Aguilera");
	tc.Add("Lizzo");
	tc.Add("Meghan Trainor");}

	{auto& tc = list.Add("Vulnerable/raw");
	tc.Add("Adele");
	tc.Add("Lana Del Rey");
	tc.Add("Birdy");
	tc.Add("Lorde");
	tc.Add("Florence + The Machine");}

	{auto& tc = list.Add("Romantic/love-driven");
	tc.Add("Selena Gomez");
	tc.Add("Ariana Grande");
	tc.Add("Halsey");
	tc.Add("Camila Cabello");}

	{auto& tc = list.Add("Failure/loser");
	tc.Add("Lorde");
	tc.Add("Demi Lovato");
	tc.Add("Selena Gomez");
	tc.Add("Sia");
	tc.Add("Amy Winehouse");}

	{auto& tc = list.Add("Spiritual/faithful");
	tc.Add("Stevie Nicks");
	tc.Add("Florence Welch");
	tc.Add("Norah Jones");
	tc.Add("Tori Amos");
	tc.Add("Sarah McLachlan");}

	{auto& tc = list.Add("Passionate/determined");
	tc.Add("P!nk");
	tc.Add("Kelly Clarkson");
	tc.Add("Beyoncé");
	tc.Add("Alicia Keys");}

	{auto& tc = list.Add("Reflective/self-reflective");
	tc.Add("Lana Del Rey");
	tc.Add("Adele");
	tc.Add("Halsey");
	tc.Add("Billie Eilish");
	tc.Add("Sara Bareilles");}

	{auto& tc = list.Add("Witty/sarcastic");
	tc.Add("Alanis Morissette");
	tc.Add("Lily Allen");
	tc.Add("Pink");
	tc.Add("Avril Lavigne");
	tc.Add("Fergie");}

	{auto& tc = list.Add("Melancholic/sad");
	tc.Add("Lana Del Rey");
	tc.Add("Adele");
	tc.Add("Billie Eilish");
	tc.Add("Halsey");
	tc.Add("Lorde");}

	{auto& tc = list.Add("Humble/down-to-earth");
	tc.Add("Norah Jones");
	tc.Add("Colbie Caillat");
	tc.Add("Ingrid Michaelson");
	tc.Add("Sara Bareilles");
	tc.Add("religion of bankersel");}

	{auto& tc = list.Add("Charismatic/charming");
	tc.Add("Selena Gomez");
	tc.Add("Ariana Grande");
	tc.Add("Jennifer Lopez");
	tc.Add("Shakira");
	tc.Add("Beyoncé");}

	{auto& tc = list.Add("Resilient/overcoming adversity");
	tc.Add("Demi Lovato");
	tc.Add("Alicia Keys");
	tc.Add("Beyoncé");
	tc.Add("Christina Aguilera");}

	{auto& tc = list.Add("Carefree/joyful");
	tc.Add("Beyoncé");
	tc.Add("Selena Gomez");
	tc.Add("Alicia Keys");
	tc.Add("Sia");}

	{auto& tc = list.Add("Dark/mysterious");
	tc.Add("Lana Del Rey");
	tc.Add("Billie Eilish");
	tc.Add("Florence Welch");
	tc.Add("Lorde");
	tc.Add("Halsey");}

	{auto& tc = list.Add("Comical/humorous");
	tc.Add("Amy Schumer");
	tc.Add("Nicki Minaj");
	tc.Add("Dua Lipa");}

	{auto& tc = list.Add("Controversial/provocative");
	tc.Add("Madonna");
	tc.Add("Miley Cyrus");
	tc.Add("Nicki Minaj");
	tc.Add("Rihanna");}

	{auto& tc = list.Add("Nostalgic/sentimental");
	tc.Add("Lana Del Rey");
	tc.Add("Sara Bareilles");
	tc.Add("Adele");
	tc.Add("Norah Jones");
	tc.Add("Norah jones");}

	{auto& tc = list.Add("Wise/philosophical");
	tc.Add("Joni Mitchell");
	tc.Add("Stevie Nicks");
	tc.Add("Alanis Morissette");
	tc.Add("religion of bankersel");
	tc.Add("Patti Smith");}

	{auto& tc = list.Add("Angry/outspoken");
	tc.Add("P!nk");
	tc.Add("Lorde");
	tc.Add("Beyoncé");
	tc.Add("Ariana Grande");
	tc.Add("Avril Lavigne");}

	{auto& tc = list.Add("Calm/peaceful");
	tc.Add("Norah Jones");
	tc.Add("Sara Bareilles");
	tc.Add("Colbie Caillat");
	tc.Add("Alicia Keys");
	tc.Add("Birdy");}

	{auto& tc = list.Add("Confident/self-assured");
	tc.Add("Beyoncé");
	tc.Add("Ariana Grande");
	tc.Add("Christina Aguilera");
	tc.Add("Rihanna");
	tc.Add("Adele");}
	
	{auto& tc = list.Add("Self-destructive/self-sabotaging");
	tc.Add("Amy Winehouse");
	tc.Add("Lana Del Rey");
	tc.Add("Sia");
	tc.Add("Demi Lovato");
	tc.Add("Halsey");}

	{auto& tc = list.Add("Hopeful/optimistic");
	tc.Add("Ariana Grande");
	tc.Add("Selena Gomez");
	tc.Add("Kelly Clarkson");
	tc.Add("Mariah Carey");}

	{auto& tc = list.Add("Fearful/anxious");
	tc.Add("Halsey");
	tc.Add("Lana Del Rey");
	tc.Add("Alessia Cara");
	tc.Add("Sia");
	tc.Add("Adele");}

	{auto& tc = list.Add("Eccentric/quirky");
	tc.Add("Björk");
	tc.Add("Florence Welch");
	tc.Add("Sia");
	tc.Add("Grimes");}

	{auto& tc = list.Add("Sensitive/emotional");
	tc.Add("Adele");
	tc.Add("Beyoncé");
	tc.Add("Selena Gomez");
	tc.Add("Halsey");
	tc.Add("Norah Jones");}

	{auto& tc = list.Add("Bitter/resentful");
	tc.Add("Alanis Morissette");
	tc.Add("Lorde");
	tc.Add("Billie Eilish");
	tc.Add("Avril Lavigne");
	tc.Add("Lana Del Rey");}

	{auto& tc = list.Add("Unique/nonconformist");
	tc.Add("Alanis Morissette");
	tc.Add("P!nk");
	tc.Add("Björk");
	tc.Add("Halsey");}

	{auto& tc = list.Add("Free-spirited/nonconformist");
	tc.Add("Miley Cyrus");
	tc.Add("Jimi Hendrix");
	tc.Add("Janis Joplin");
	tc.Add("Pink Floyd");
	tc.Add("Led Zeppelin");}

	{auto& tc = list.Add("Sultry/seductive");
	tc.Add("Lana Del Rey");
	tc.Add("Beyoncé");
	tc.Add("Rihanna");
	tc.Add("Madonna");
	tc.Add("Selena Gomez");}

	{auto& tc = list.Add("Inspirational/motivational");
	tc.Add("Demi Lovato");
	tc.Add("Alicia Keys");
	tc.Add("Christina Aguilera");
	tc.Add("Beyoncé");}

	{auto& tc = list.Add("Authentic/real");
	tc.Add("Adele");
	tc.Add("Pink");
	tc.Add("Beyoncé");
	tc.Add("Alanis Morissette");
	tc.Add("Ariana Grande");}

	{auto& tc = list.Add("Mysterious/enigmatic");
	tc.Add("Lana Del Rey");
	tc.Add("Lorde");
	tc.Add("Billie Eilish");
	tc.Add("Sia");
	tc.Add("Florence Welch");}

	{auto& tc = list.Add("Carefree/bohemian");
	tc.Add("Florence Welch");
	tc.Add("Lana Del Rey");
	tc.Add("Norah Jones");
	tc.Add("Sia");
	tc.Add("Stevie Nicks");}

	{auto& tc = list.Add("Street-smart/tough");
	tc.Add("Cardi B");
	tc.Add("Beyoncé");
	tc.Add("Nicki Minaj");
	tc.Add("Rihanna");
	tc.Add("Missy Elliott");}

	{auto& tc = list.Add("Romantic/idealistic");
	tc.Add("Ariana Grande");
	tc.Add("Beyoncé");
	tc.Add("Selena Gomez");
	tc.Add("Lana Del Rey");}

	{auto& tc = list.Add("Nurturing/motherly");
	tc.Add("Dolly Parton");
	tc.Add("Shania Twain");
	tc.Add("Adele");
	tc.Add("Norah Jones");
	tc.Add("Christina Aguilera");}

	{auto& tc = list.Add("Dark/tormented");
	tc.Add("Lana Del Rey");
	tc.Add("Billie Eilish");
	tc.Add("Lorde");
	tc.Add("Halsey");
	tc.Add("Adele");}

	{auto& tc = list.Add("Remorseful/regretful");
	tc.Add("Adele");
	tc.Add("Beyoncé");
	tc.Add("Pink");
	tc.Add("Alanis Morissette");
	tc.Add("Lana Del Rey");}

	{auto& tc = list.Add("Bold/brave");
	tc.Add("P!nk");
	tc.Add("Beyoncé");
	tc.Add("Demi Lovato");}

	{auto& tc = list.Add("Outcast/rebel");
	tc.Add("Billie Eilish");
	tc.Add("Demi Lovato");
	tc.Add("Miley Cyrus");
	tc.Add("Alanis Morissette");
	tc.Add("Beyoncé");}

	{auto& tc = list.Add("Lost/disconnected");
	tc.Add("Lana Del Rey");
	tc.Add("Adele");
	tc.Add("Florence + The Machine");
	tc.Add("Halsey");
	tc.Add("Lorde");}

	{auto& tc = list.Add("Tough/badass");
	tc.Add("Beyoncé");
	tc.Add("Rihanna");
	tc.Add("P!nk");
	tc.Add("Madonna");
	tc.Add("Cardi B");}

	{auto& tc = list.Add("Sincere/genuine");
	tc.Add("Adele");
	tc.Add("Kelly Clarkson");
	tc.Add("Sara Bareilles");
	tc.Add("Norah Jones");
	tc.Add("Christina Aguilera");}

	{auto& tc = list.Add("Honest/vulnerable");
	tc.Add("Adele");
	tc.Add("Halsey");
	tc.Add("Lana Del Rey");
	tc.Add("Lorde");
	tc.Add("Sia");}

	{auto& tc = list.Add("Innocent/naive");
	tc.Add("Selena Gomez");
	tc.Add("Ariana Grande");
	tc.Add("Demi Lovato");}

	{auto& tc = list.Add("Bold/risk-taking");
	tc.Add("Pink");
	tc.Add("Madonna");
	tc.Add("Beyoncé");
	tc.Add("Miley Cyrus");}
	
	ASSERT(list.GetCount() == GetTypecastCount());
	return list;
}

VectorMap<String,Vector<String>>& GetTypecastRappersFemale() {
	thread_local static VectorMap<String,Vector<String>> list;
	if (!list.IsEmpty()) return list;
	

	{auto& tc = list.Add("Heartbroken/lovesick");
	tc.Add("Missy Elliott");
	tc.Add("Lauryn Hill");
	tc.Add("Queen Latifah");
	tc.Add("Aaliyah");
	tc.Add("Lil Kim");
	tc.Add("Eve");}

	{auto& tc = list.Add("Rebel/anti-establishment");
	tc.Add("Cardi B");
	tc.Add("Nicki Minaj");
	tc.Add("Lauryn Hill");
	tc.Add("Queen Latifah");}

	{auto& tc = list.Add("Political activist");
	tc.Add("Lauryn Hill");
	tc.Add("Queen Latifah");
	tc.Add("Eve");
	tc.Add("Dead Prez");
	tc.Add("Angel Haze");}

	{auto& tc = list.Add("Social justice advocate");
	tc.Add("Lauryn Hill");
	tc.Add("Queen Latifah");
	tc.Add("Angel Haze");
	tc.Add("Rapsody");
	tc.Add("Noname");
	tc.Add("Jean Grae");}

	{auto& tc = list.Add("Party/club");
	tc.Add("Missy Elliott");
	tc.Add("Cardi B");
	tc.Add("Nicki Minaj");
	tc.Add("Lil Kim");
	tc.Add("Salt-N-Pepa");
	tc.Add("Eve");
	tc.Add("Megan Thee Stallion");
	tc.Add("Da Brat");}

	{auto& tc = list.Add("Hopeful/dreamer");
	tc.Add("Lauryn Hill");
	tc.Add("Nicki Minaj");
	tc.Add("J. Cole");
	tc.Add("Rapsody");
	tc.Add("Lizzo");}

	{auto& tc = list.Add("Confident/empowered");
	tc.Add("Missy Elliott");
	tc.Add("Cardi B");
	tc.Add("Nicki Minaj");
	tc.Add("Queen Latifah");
	tc.Add("Lizzo");
	tc.Add("Eve");
	tc.Add("City Girls");}

	{auto& tc = list.Add("Vulnerable/raw");
	tc.Add("Lauryn Hill");
	tc.Add("Noname");
	tc.Add("Missy Elliott");
	tc.Add("Nicki Minaj");
	tc.Add("Angel Haze");}

	{auto& tc = list.Add("Romantic/love-driven");
	tc.Add("Cardi B");
	tc.Add("Nicki Minaj");
	tc.Add("Lauryn Hill");
	tc.Add("Lil Kim");
	tc.Add("Queen Latifah");
	tc.Add("Aaliyah");}

	{auto& tc = list.Add("Failure/loser");
	tc.Add("Missy Elliott");
	tc.Add("Cardi B");
	tc.Add("Nicki Minaj");
	tc.Add("Lauryn Hill");
	tc.Add("Lil Kim");}

	{auto& tc = list.Add("Spiritual/faithful");
	tc.Add("Lauryn Hill");
	tc.Add("Queen Latifah");
	tc.Add("Rapsody");
	tc.Add("Noname");
	tc.Add("Aaliyah");
	tc.Add("Lizzo");}

	{auto& tc = list.Add("Passionate/determined");
	tc.Add("Lauryn Hill");
	tc.Add("Noname");
	tc.Add("Angel Haze");
	tc.Add("Rapsody");
	tc.Add("Nicki Minaj");}

	{auto& tc = list.Add("Reflective/self-reflective");
	tc.Add("Lauryn Hill");
	tc.Add("Noname");
	tc.Add("Nicki Minaj");
	tc.Add("Rapsody");
	tc.Add("Angel Haze");
	tc.Add("Queen Latifah");}

	{auto& tc = list.Add("Witty/sarcastic");
	tc.Add("Missy Elliott");
	tc.Add("Cardi B");
	tc.Add("Nicki Minaj");
	tc.Add("Lil Kim");
	tc.Add("City Girls");}

	{auto& tc = list.Add("Melancholic/sad");
	tc.Add("Lauryn Hill");
	tc.Add("Noname");
	tc.Add("Nicki Minaj");
	tc.Add("Aaliyah");
	tc.Add("Lizzo");}

	{auto& tc = list.Add("Humble/down-to-earth");
	tc.Add("Lauryn Hill");
	tc.Add("Noname");
	tc.Add("Rapsody");
	tc.Add("Aaliyah");
	tc.Add("Megan Thee Stallion");}

	{auto& tc = list.Add("Charismatic/charming");
	tc.Add("Missy Elliott");
	tc.Add("Cardi B");
	tc.Add("Nicki Minaj");
	tc.Add("Lil Kim");
	tc.Add("Lauryn Hill");
	tc.Add("Rapsody");
	tc.Add("Aaliyah");
	tc.Add("Queen Latifah");}

	{auto& tc = list.Add("Resilient/overcoming adversity");
	tc.Add("Lauryn Hill");
	tc.Add("Rapsody");
	tc.Add("Megan Thee Stallion");
	tc.Add("Nicki Minaj");
	tc.Add("Lizzo");}

	{auto& tc = list.Add("Carefree/joyful");
	tc.Add("Missy Elliott");
	tc.Add("Cardi B");
	tc.Add("Nicki Minaj");
	tc.Add("Eve");
	tc.Add("Megan Thee Stallion");
	tc.Add("Da Brat");}

	{auto& tc = list.Add("Dark/mysterious");
	tc.Add("Nicki Minaj");
	tc.Add("Lil Kim");
	tc.Add("Rapsody");
	tc.Add("Angel Haze");
	tc.Add("Aaliyah");}

	{auto& tc = list.Add("Comical/humorous");
	tc.Add("Missy Elliott");
	tc.Add("Eve");
	tc.Add("Da Brat");
	tc.Add("City Girls");}

	{auto& tc = list.Add("Controversial/provocative");
	tc.Add("Cardi B");
	tc.Add("Nicki Minaj");
	tc.Add("Lil Kim");
	tc.Add("Da Brat");
	tc.Add("Megan Thee Stallion");}

	{auto& tc = list.Add("Nostalgic/sentimental");
	tc.Add("Lauryn Hill");
	tc.Add("Queen Latifah");
	tc.Add("Noname");
	tc.Add("Aaliyah");
	tc.Add("Lil Kim");}

	{auto& tc = list.Add("Wise/philosophical");
	tc.Add("Lauryn Hill");
	tc.Add("Noname");
	tc.Add("Queen Latifah");
	tc.Add("Rapsody");
	tc.Add("Angel Haze");}

	{auto& tc = list.Add("Angry/outspoken");
	tc.Add("Cardi B");
	tc.Add("Missy Elliott");
	tc.Add("Da Brat");
	tc.Add("Megan Thee Stallion");
	tc.Add("Nicki Minaj");}

	{auto& tc = list.Add("Calm/peaceful");
	tc.Add("Lauryn Hill");
	tc.Add("Noname");
	tc.Add("Angel Haze");
	tc.Add("Rapsody");
	tc.Add("Aaliyah");
	tc.Add("Queen Latifah");}

	{auto& tc = list.Add("Confident/self-assured");
	tc.Add("Nicki Minaj");
	tc.Add("Missy Elliott");
	tc.Add("Busta Rhymes");
	tc.Add("Cardi B");
	tc.Add("Eve");
	tc.Add("Megan Thee Stallion");
	tc.Add("Lil Kim");}

	{auto& tc = list.Add("Self-destructive/self-sabotaging");
	tc.Add("Lauryn Hill");
	tc.Add("Rapsody");
	tc.Add("Noname");
	tc.Add("Megan Thee Stallion");
	tc.Add("Lil Kim");
	tc.Add("Lizzo");}

	{auto& tc = list.Add("Hopeful/optimistic");
	tc.Add("J. Cole");
	tc.Add("Rapsody");
	tc.Add("Lauryn Hill");
	tc.Add("Megan Thee Stallion");
	tc.Add("Queen Latifah");}

	{auto& tc = list.Add("Fearful/anxious");
	tc.Add("Missy Elliott");
	tc.Add("Nicki Minaj");
	tc.Add("Lil Kim");
	tc.Add("Cardi B");
	tc.Add("Megan Thee Stallion");}

	{auto& tc = list.Add("Eccentric/quirky");
	tc.Add("Missy Elliott");
	tc.Add("Lauryn Hill");
	tc.Add("Angel Haze");
	tc.Add("Busta Rhymes");
	tc.Add("Rapsody");}

	{auto& tc = list.Add("Sensitive/emotional");
	tc.Add("Lauryn Hill");
	tc.Add("Noname");
	tc.Add("Nicki Minaj");
	tc.Add("Aaliyah");
	tc.Add("Megan Thee Stallion");}

	{auto& tc = list.Add("Bitter/resentful");
	tc.Add("Noname");
	tc.Add("Missy Elliott");
	tc.Add("Cardi B");
	tc.Add("Lil Kim");
	tc.Add("Queen Latifah");}

	{auto& tc = list.Add("Unique/nonconformist");
	tc.Add("Noname");
	tc.Add("Lauryn Hill");
	tc.Add("Angel Haze");
	tc.Add("Nicki Minaj");
	tc.Add("Lil Kim");}

	{auto& tc = list.Add("Free-spirited/nonconformist");
	tc.Add("Lauryn Hill");
	tc.Add("Noname");
	tc.Add("Angel Haze");
	tc.Add("Missy Elliott");
	tc.Add("Nicki Minaj");
	tc.Add("Queen Latifah");}

	{auto& tc = list.Add("Sultry/seductive");
	tc.Add("Lil Kim");
	tc.Add("Nicki Minaj");
	tc.Add("Cardi B");
	tc.Add("Eve");
	tc.Add("Aaliyah");}

	{auto& tc = list.Add("Inspirational/motivational");
	tc.Add("Rapsody");
	tc.Add("Lauryn Hill");
	tc.Add("Nicki Minaj");
	tc.Add("Missy Elliott");
	tc.Add("Megan Thee Stallion");}

	{auto& tc = list.Add("Authentic/real");
	tc.Add("Lauryn Hill");
	tc.Add("Rapsody");
	tc.Add("Angel Haze");
	tc.Add("Noname");
	tc.Add("Queen Latifah");}

	{auto& tc = list.Add("Mysterious/enigmatic");
	tc.Add("Lauryn Hill");
	tc.Add("Noname");
	tc.Add("Angel Haze");
	tc.Add("Rapsody");
	tc.Add("Aaliyah");}

	{auto& tc = list.Add("Carefree/bohemian");
	tc.Add("Lauryn Hill");
	tc.Add("Noname");
	tc.Add("Angel Haze");
	tc.Add("Queen Latifah");
	tc.Add("Janelle Monáe");}

	{auto& tc = list.Add("Street-smart/tough");
	tc.Add("Missy Elliott");
	tc.Add("Nicki Minaj");
	tc.Add("Lil Kim");
	tc.Add("Eve");
	tc.Add("Rapsody");
	tc.Add("Queen Latifah");}

	{auto& tc = list.Add("Romantic/idealistic");
	tc.Add("Lauryn Hill");
	tc.Add("Aaliyah");
	tc.Add("Meg Thee Stallion");
	tc.Add("Nicki Minaj");
	tc.Add("Lil Kim");
	tc.Add("Queen Latifah");}

	{auto& tc = list.Add("Nurturing/motherly");
	tc.Add("Missy Elliott");
	tc.Add("Erykah Badu");
	tc.Add("Queen Latifah");
	tc.Add("Lauryn Hill");
	tc.Add("Rapsody");
	tc.Add("MC Lyte");}

	{auto& tc = list.Add("Dark/tormented");
	tc.Add("Nicki Minaj");
	tc.Add("Lauryn Hill");
	tc.Add("Noname");
	tc.Add("Megan Thee Stallion");
	tc.Add("Angel Haze");}

	{auto& tc = list.Add("Remorseful/regretful");
	tc.Add("Lauryn Hill");
	tc.Add("Aaliyah");
	tc.Add("Noname");
	tc.Add("Nicki Minaj");
	tc.Add("Megan Thee Stallion");}

	{auto& tc = list.Add("Bold/brave");
	tc.Add("Missy Elliott");
	tc.Add("Cardi B");
	tc.Add("Nicki Minaj");
	tc.Add("Lil Kim");
	tc.Add("Queen Latifah");
	tc.Add("Angel Haze");
	tc.Add("Azealia Banks");}

	{auto& tc = list.Add("Outcast/rebel");
	tc.Add("Missy Elliott");
	tc.Add("Nicki Minaj");
	tc.Add("Lil Kim");
	tc.Add("Queen Latifah");
	tc.Add("Angel Haze");
	tc.Add("Azealia Banks");}

	{auto& tc = list.Add("Lost/disconnected");
	tc.Add("Lauryn Hill");
	tc.Add("Noname");
	tc.Add("Megan Thee Stallion");
	tc.Add("Erykah Badu");
	tc.Add("Aaliyah");}

	{auto& tc = list.Add("Tough/badass");
	tc.Add("Cardi B");
	tc.Add("Missy Elliott");
	tc.Add("Nicki Minaj");
	tc.Add("Lil Kim");
	tc.Add("Megan Thee Stallion");
	tc.Add("Rapsody");
	tc.Add("Da Brat");}

	{auto& tc = list.Add("Sincere/genuine");
	tc.Add("Lauryn Hill");
	tc.Add("Rapsody");
	tc.Add("Noname");
	tc.Add("Angel Haze");
	tc.Add("Queen Latifah");}

	{auto& tc = list.Add("Honest/vulnerable");
	tc.Add("Noname");
	tc.Add("Lauryn Hill");
	tc.Add("Rapsody");
	tc.Add("Aaliyah");
	tc.Add("Megan Thee Stallion");}

	{auto& tc = list.Add("Innocent/naive");
	tc.Add("Missy Elliott");
	tc.Add("Aaliyah");
	tc.Add("Janelle Monáe");
	tc.Add("Noname");}

	{auto& tc = list.Add("Bold/risk-taking");
	tc.Add("Missy Elliott");
	tc.Add("Cardi B");
	tc.Add("Nicki Minaj");
	tc.Add("Busta Rhymes");
	tc.Add("Angel Haze");
	tc.Add("Rapsody");}

	ASSERT(list.GetCount() == GetTypecastCount());
	return list;
}

VectorMap<String,Vector<String>>& GetTypecastSingersMale() {
	thread_local static VectorMap<String,Vector<String>> list;
	if (!list.IsEmpty()) return list;
	
	{auto& tc = list.GetAdd("Heartbroken/lovesick");
	tc.Add("Sam Smith");
	tc.Add("Harry Styles");
	tc.Add("Ed Sheeran");
	tc.Add("John Mayer");
	tc.Add("The Weeknd");}

	{auto& tc = list.GetAdd("Rebel/anti-establishment");
	tc.Add("John Lennon");
	tc.Add("Kurt Cobain");
	tc.Add("Bob Dylan");
	tc.Add("Red Hot Chili Peppers");
	tc.Add("Freddie Mercury");}

	{auto& tc = list.GetAdd("Political activist");
	tc.Add("Bob Marley");
	tc.Add("Michael Franti");
	tc.Add("Neil Young");
	tc.Add("Bono");
	tc.Add("Woody Guthrie");}

	{auto& tc = list.GetAdd("Social justice advocate");
	tc.Add("Marvin Gaye");
	tc.Add("Stevie Wonder");
	tc.Add("John Legend");
	tc.Add("Kendrick Lamar");
	tc.Add("Common");}

	{auto& tc = list.GetAdd("Party/club");
	tc.Add("Pitbull");
	tc.Add("Usher");
	tc.Add("Justin Timberlake");
	tc.Add("Bruno Mars");
	tc.Add("Jason Derulo");}

	{auto& tc = list.GetAdd("Hopeful/dreamer");
	tc.Add("Imagine Dragons");
	tc.Add("Coldplay");
	tc.Add("Phillip Phillips");
	tc.Add("John Mayer");
	tc.Add("Red Hot Chili Peppers");}

	{auto& tc = list.GetAdd("Confident/empowered");
	tc.Add("Red Hot Chili Peppers");
	tc.Add("Van Halen");
	tc.Add("Green Day");
	tc.Add("Blink-182");
	tc.Add("Nirvana");}

	{auto& tc = list.GetAdd("Vulnerable/raw");
	tc.Add("Red Hot Chili Peppers");
	tc.Add("Sam Smith");
	tc.Add("Ed Sheeran");
	tc.Add("Sia");
	tc.Add("Amy Winehouse");}

	{auto& tc = list.GetAdd("Romantic/love-driven");
	tc.Add("Red Hot Chili Peppers");
	tc.Add("Ed Sheeran");
	tc.Add("John Legend");
	tc.Add("John Mayer");
	tc.Add("Sam Smith");}

	{auto& tc = list.GetAdd("Failure/loser");
	tc.Add("Red Hot Chili Peppers");
	tc.Add("Air Supply");
	tc.Add("Bon Jovi");
	tc.Add("Shawn Mendes");
	tc.Add("Fall Out Boy");}

	{auto& tc = list.GetAdd("Spiritual/faithful");
	tc.Add("Red Hot Chili Peppers");
	tc.Add("Johnny Cash");
	tc.Add("Jason Mraz");
	tc.Add("Hillsong United");
	tc.Add("Chris Tomlin");}

	{auto& tc = list.GetAdd("Passionate/determined");
	tc.Add("Red Hot Chili Peppers");
	tc.Add("Elton John");
	}

	{auto& tc = list.GetAdd("Reflective/self-reflective");
	tc.Add("Red Hot Chili Peppers");
	tc.Add("Leonard Cohen");
	tc.Add("Bob Dylan");
	tc.Add("James Taylor");
	tc.Add("Tracy Chapman");
	tc.Add("Jack Johnson");}

	{auto& tc = list.GetAdd("Witty/sarcastic");
	tc.Add("Eminem");
	tc.Add("Kendrick Lamar");
	tc.Add("Arctic Monkeys");
	tc.Add("Childish Gambino");}

	{auto& tc = list.GetAdd("Melancholic/sad");
	tc.Add("Hozier");
	tc.Add("Lana Del Rey");
	tc.Add("The Weeknd");}

	{auto& tc = list.GetAdd("Humble/down-to-earth");
	tc.Add("Ed Sheeran");
	tc.Add("Bruno Mars");
	tc.Add("Jason Mraz");
	tc.Add("John Mayer");}

	{auto& tc = list.GetAdd("Charismatic/charming");
	tc.Add("Bruno Mars");
	tc.Add("Justin Timberlake");
	tc.Add("Frank Sinatra");
	tc.Add("Michael Buble");
	tc.Add("Harry Connick Jr.");}

	{auto& tc = list.GetAdd("Resilient/overcoming adversity");
	tc.Add("Eminem");}

	{auto& tc = list.GetAdd("Carefree/joyful");
	tc.Add("Maroon 5");
	tc.Add("Jason Mraz");
	tc.Add("Jack Johnson");}

	{auto& tc = list.GetAdd("Dark/mysterious");
	tc.Add("The Weeknd");
	tc.Add("David Bowie");}

	{auto& tc = list.GetAdd("Comical/humorous");
	tc.Add("Weird Al Yankovic");
	tc.Add("Flight of the Conchords");
	tc.Add("Stephen Lynch");
	tc.Add("Rodney Carrington");
	tc.Add("The Lonely Island");}

	{auto& tc = list.GetAdd("Controversial/provocative");
	tc.Add("Marilyn Manson");
	tc.Add("Die Antwoord");}

	{auto& tc = list.GetAdd("Nostalgic/sentimental");
	tc.Add("The Beatles");
	tc.Add("Fleetwood Mac");
	tc.Add("Elton John");
	tc.Add("Billy Joel");
	tc.Add("James Taylor");}

	{auto& tc = list.GetAdd("Wise/philosophical");
	tc.Add("Bob Dylan");
	tc.Add("Leonard Cohen");
	tc.Add("Neil Young");
	tc.Add("Paul Simon");
	tc.Add("Tom Petty");}

	{auto& tc = list.GetAdd("Angry/outspoken");
	tc.Add("Rage Against the Machine");
	tc.Add("Green Day");
	tc.Add("Eminem");
	tc.Add("Kanye West");
	tc.Add("System of a Down");}

	{auto& tc = list.GetAdd("Calm/peaceful");
	tc.Add("Jack Johnson");
	tc.Add("Jason Mraz");
	tc.Add("James Taylor");
	tc.Add("John Mayer");
	tc.Add("Ben Howard");}

	{auto& tc = list.GetAdd("Confident/self-assured");
	tc.Add("Eminem");}

	{auto& tc = list.GetAdd("Self-destructive/self-sabotaging");
	tc.Add("Nirvana");
	tc.Add("Kurt Cobain");
	tc.Add("Nine Inch Nails");}

	{auto& tc = list.GetAdd("Hopeful/optimistic");
	tc.Add("Coldplay");
	tc.Add("Phillip Phillips");
	tc.Add("Imagine Dragons");
	tc.Add("U2");
	tc.Add("OneRepublic");}

	{auto& tc = list.GetAdd("Fearful/anxious");
	tc.Add("Twenty One Pilots");}

	{auto& tc = list.GetAdd("Eccentric/quirky");
	tc.Add("David Bowie");
	tc.Add("Prince");
	tc.Add("Tame Impala");}

	{auto& tc = list.GetAdd("Sensitive/emotional");
	tc.Add("James Bay");
	tc.Add("James Taylor");
	tc.Add("Passenger");}

	{auto& tc = list.GetAdd("Bitter/resentful");
	tc.Add("Nirvana");
	tc.Add("Nine Inch Nails");}

	{auto& tc = list.GetAdd("Unique/nonconformist");
	tc.Add("David Bowie");
	tc.Add("Prince");}

	{auto& tc = list.GetAdd("Free-spirited/nonconformist");
	tc.Add("Johnny Cash");
	tc.Add("Bob Dylan");
	tc.Add("Jimi Hendrix");}

	{auto& tc = list.GetAdd("Sultry/seductive");
	tc.Add("Chris Isaak");
	tc.Add("Frank Sinatra");}

	{auto& tc = list.GetAdd("Inspirational/motivational");
	tc.Add("Michael Jackson");
	tc.Add("Imagine Dragons");}

	{auto& tc = list.GetAdd("Authentic/real");
	tc.Add("John Mayer");
	tc.Add("Jason Mraz");
	tc.Add("Ed Sheeran");
	tc.Add("Hozier");}

	{auto& tc = list.GetAdd("Mysterious/enigmatic");
	tc.Add("Billie Eilish");
	tc.Add("David Bowie");
	tc.Add("Florence + The Machine");
	tc.Add("The Weeknd");}

	{auto& tc = list.GetAdd("Carefree/bohemian");
	tc.Add("Fleetwood Mac");
	tc.Add("Grateful Dead");
	tc.Add("Jack Johnson");
	tc.Add("Ben Howard");
	tc.Add("Bob Marley");}

	{auto& tc = list.GetAdd("Street-smart/tough");
	tc.Add("N.W.A");
	tc.Add("Tupac Shakur");
	tc.Add("Eminem");
	tc.Add("Jay-Z");
	tc.Add("The Notorious B.I.G.");}

	{auto& tc = list.GetAdd("Romantic/idealistic");
	tc.Add("John Legend");
	tc.Add("John Mayer");
	tc.Add("Bruno Mars");
	tc.Add("Sam Smith");
	tc.Add("Rex Orange County");}

	{auto& tc = list.GetAdd("Nurturing/motherly");
	tc.Add("Red Hot Chili Peppers");}

	{auto& tc = list.GetAdd("Dark/tormented");
	tc.Add("The Weeknd");
	tc.Add("Kanye West");
	tc.Add("Kendrick Lamar");}

	{auto& tc = list.GetAdd("Remorseful/regretful");
	tc.Add("Justin Bieber");
	tc.Add("Eminem");
	tc.Add("Maroon 5");
	tc.Add("Coldplay");}

	{auto& tc = list.GetAdd("Bold/brave");
	tc.Add("Red Hot Chili Peppers");}

	{auto& tc = list.GetAdd("Outcast/rebel");
	tc.Add("Nirvana");
	tc.Add("Green Day");
	tc.Add("Eminem");
	tc.Add("Kurt Cobain");}

	{auto& tc = list.GetAdd("Lost/disconnected");
	tc.Add("Radiohead");
	tc.Add("Tame Impala");
	tc.Add("The Weeknd");}

	{auto& tc = list.GetAdd("Tough/badass");
	tc.Add("AC/DC");
	tc.Add("Metallica");
	tc.Add("Lynyrd Skynyrd");
	tc.Add("Guns N' Roses");
	tc.Add("Queen");}

	{auto& tc = list.GetAdd("Sincere/genuine");
	tc.Add("Ed Sheeran");
	tc.Add("John Mayer");
	tc.Add("Jason Mraz");
	tc.Add("James Bay");
	tc.Add("Hozier");}

	{auto& tc = list.GetAdd("Honest/vulnerable");
	tc.Add("Sam Smith");
	tc.Add("James Bay");
	tc.Add("Ed Sheeran");}

	{auto& tc = list.GetAdd("Innocent/naive");
	tc.Add("One Direction");
	tc.Add("Shawn Mendes");
	tc.Add("Justin Bieber");
	tc.Add("Bruno Mars");}

	{auto& tc = list.GetAdd("Bold/risk-taking");
	tc.Add("Kanye West");}




	{auto& tc = list.GetAdd("Heartbroken/lovesick");
	tc.Add("Red Hot Chili Peppers");
	tc.Add("Nirvana");
	tc.Add("The Beatles");
	tc.Add("Bob Dylan");
	tc.Add("The Doors");}

	{auto& tc = list.GetAdd("Rebel/anti-establishment");
	tc.Add("The Offspring");
	tc.Add("The Clash");
	tc.Add("Rage Against the Machine");
	tc.Add("N.W.A");
	tc.Add("Public Enemy");
	tc.Add("Green Day");}

	{auto& tc = list.GetAdd("Political activist");
	tc.Add("Bob Marley");
	tc.Add("Bob Dylan");
	tc.Add("Public Enemy");
	tc.Add("Rage Against the Machine");
	tc.Add("The Clash");}

	{auto& tc = list.GetAdd("Social justice advocate");
	tc.Add("Bob Marley");
	tc.Add("Public Enemy");
	tc.Add("The Clash");
	tc.Add("Marvin Gaye");
	tc.Add("N.W.A");}

	{auto& tc = list.GetAdd("Party/club");
	tc.Add("The Offspring");
	tc.Add("Limp Bizkit");
	tc.Add("Jay-Z");
	tc.Add("Madonna");
	tc.Add("The Beastie Boys");
	tc.Add("Beyonce");}

	{auto& tc = list.GetAdd("Hopeful/dreamer");
	tc.Add("The Beach Boys");
	tc.Add("ABBA");
	tc.Add("Elton John");
	tc.Add("Bon Jovi");
	tc.Add("U2");}

	{auto& tc = list.GetAdd("Confident/empowered");
	tc.Add("The Offspring");
	tc.Add("Queen");
	tc.Add("AC/DC");}

	{auto& tc = list.GetAdd("Vulnerable/raw");
	tc.Add("Red Hot Chili Peppers");
	tc.Add("Bon Iver");
	tc.Add("The National");}

	{auto& tc = list.GetAdd("Romantic/love-driven");
	tc.Add("The Offspring");
	tc.Add("The Beatles");
	tc.Add("Ed Sheeran");
	tc.Add("Coldplay");}

	{auto& tc = list.GetAdd("Failure/loser");
	tc.Add("The Offspring");
	tc.Add("Green Day");
	tc.Add("The Smiths");
	tc.Add("Radiohead");
	tc.Add("Blink-182");
	tc.Add("Morrissey");}

	{auto& tc = list.GetAdd("Spiritual/faithful");
	tc.Add("U2");
	tc.Add("Bob Dylan");
	tc.Add("Sufjan Stevens");
	tc.Add("Mumford & Sons");}

	{auto& tc = list.GetAdd("Passionate/determined");
	tc.Add("The Offspring");
	tc.Add("Bruce Springsteen");
	tc.Add("Nirvana");}

	{auto& tc = list.GetAdd("Reflective/self-reflective");
	tc.Add("Radiohead");
	tc.Add("The National");
	tc.Add("Red Hot Chili Peppers");}

	{auto& tc = list.GetAdd("Witty/sarcastic");
	tc.Add("The Offspring");
	tc.Add("Flight of the Conchords");
	tc.Add("The Lonely Island");
	tc.Add("Weird Al Yankovic");
	tc.Add("Tenacious D");
	tc.Add("The Lonely Island");}

	{auto& tc = list.GetAdd("Melancholic/sad");
	tc.Add("The Smiths");
	tc.Add("Joy Division");
	tc.Add("The Cure");}

	{auto& tc = list.GetAdd("Humble/down-to-earth");
	tc.Add("Johnny Cash");
	tc.Add("Bob Dylan");
	tc.Add("Mumford & Sons");
	tc.Add("Ed Sheeran");}

	{auto& tc = list.GetAdd("Charismatic/charming");
	tc.Add("The Rolling Stones");
	tc.Add("Justin Timberlake");
	tc.Add("Maroon 5");}

	{auto& tc = list.GetAdd("Resilient/overcoming adversity");
	tc.Add("The Offspring");
	tc.Add("Red Hot Chili Peppers");
	tc.Add("The Doors");
	tc.Add("Green Day");
	tc.Add("Foo Fighters");
	}

	{auto& tc = list.GetAdd("Carefree/joyful");
	tc.Add("The Offspring");
	tc.Add("The Beach Boys");
	tc.Add("Jack Johnson");
	tc.Add("The Beatles");}

	{auto& tc = list.GetAdd("Dark/mysterious");
	tc.Add("Joy Division");
	tc.Add("Nine Inch Nails");
	tc.Add("Radiohead");
	tc.Add("The Smashing Pumpkins");
	tc.Add("PJ Harvey");}

	{auto& tc = list.GetAdd("Comical/humorous");
	tc.Add("The Offspring");
	tc.Add("Flight of the Conchords");
	tc.Add("Tenacious D");
	tc.Add("The Lonely Island");
	tc.Add("Tim Minchin");}

	{auto& tc = list.GetAdd("Controversial/provocative");
	tc.Add("The Offspring");
	tc.Add("Marilyn Manson");
	tc.Add("Rage Against the Machine");}

	{auto& tc = list.GetAdd("Nostalgic/sentimental");
	tc.Add("The Offspring");
	tc.Add("The Beatles");
	tc.Add("Fleetwood Mac");
	tc.Add("Queen");
	tc.Add("The Eagles");
	tc.Add("Red Hot Chili Peppers");
	tc.Add("Bon Jovi");}

	{auto& tc = list.GetAdd("Wise/philosophical");
	tc.Add("The Offspring");
	tc.Add("Bob Dylan");
	tc.Add("Leonard Cohen");
	tc.Add("Simon & Garfunkel");}

	{auto& tc = list.GetAdd("Angry/outspoken");
	tc.Add("The Offspring");
	tc.Add("Red Hot Chili Peppers");
	tc.Add("Rage Against the Machine");
	tc.Add("Green Day");}

	{auto& tc = list.GetAdd("Calm/peaceful");
	tc.Add("Jack Johnson");
	tc.Add("Red Hot Chili Peppers");
	tc.Add("Bob Marley");
	tc.Add("Feist");
	tc.Add("Ben Harper");}

	{auto& tc = list.GetAdd("Self-destructive/self-sabotaging");
	tc.Add("Kurt Cobain");
	tc.Add("Nirvana");
	tc.Add("Red Hot Chili Peppers");
	}

	{auto& tc = list.GetAdd("Hopeful/optimistic");
	tc.Add("U2");
	tc.Add("Coldplay");
	tc.Add("Imagine Dragons");
	tc.Add("The Lumineers");
	tc.Add("Mumford & Sons");}

	{auto& tc = list.GetAdd("Fearful/anxious");
	tc.Add("The Offspring");
	tc.Add("Radiohead");
	tc.Add("The National");
	tc.Add("Arcade Fire");
	tc.Add("Bon Iver");}

	{auto& tc = list.GetAdd("Eccentric/quirky");
	tc.Add("The Offspring");
	tc.Add("Red Hot Chili Peppers");
	tc.Add("The Flaming Lips");
	tc.Add("David Bowie");
	tc.Add("They Might Be Giants");
	tc.Add("Neutral Milk Hotel");}

	{auto& tc = list.GetAdd("Sensitive/emotional");
	tc.Add("Red Hot Chili Peppers");
	tc.Add("Bon Iver");
	tc.Add("Sam Smith");
	tc.Add("James Blake");}

	{auto& tc = list.GetAdd("Bitter/resentful");
	tc.Add("Marilyn Manson");
	tc.Add("The Offspring");
	}

	{auto& tc = list.GetAdd("Unique/nonconformist");
	tc.Add("The Offspring");
	tc.Add("David Bowie");
	tc.Add("Queen");
	tc.Add("The Velvet Underground");
	tc.Add("Red Hot Chili Peppers");
	tc.Add("Prince");}

	{auto& tc = list.GetAdd("Free-spirited/nonconformist");
	tc.Add("The Offspring");
	tc.Add("Bob Dylan");
	tc.Add("Red Hot Chili Peppers");
	tc.Add("The Grateful Dead");
	tc.Add("Fleetwood Mac");
	tc.Add("Joan Baez");}

	{auto& tc = list.GetAdd("Sultry/seductive");
	tc.Add("Billie Holiday");
	tc.Add("The Weeknd");
	tc.Add("Marvin Gaye");}

	{auto& tc = list.GetAdd("Inspirational/motivational");
	tc.Add("U2");
	tc.Add("Coldplay");
	tc.Add("Bruce Springsteen");}

	{auto& tc = list.GetAdd("Authentic/real");
	tc.Add("The Offspring");
	tc.Add("Bon Iver");
	tc.Add("Frank Ocean");
	tc.Add("Kendrick Lamar");}

	{auto& tc = list.GetAdd("Mysterious/enigmatic");
	tc.Add("Radiohead");
	tc.Add("The Black Keys");
	tc.Add("Tom Waits");
	tc.Add("Portishead");}

	{auto& tc = list.GetAdd("Carefree/bohemian");
	tc.Add("The Offspring");
	tc.Add("Edward Sharpe and the Magnetic Zeros");
	tc.Add("Bob Marley");
	tc.Add("Jack Johnson");
	tc.Add("The Grateful Dead");
	tc.Add("The Beatles");}

	{auto& tc = list.GetAdd("Street-smart/tough");
	tc.Add("The Offspring");
	tc.Add("Red Hot Chili Peppers");
	}

	{auto& tc = list.GetAdd("Romantic/idealistic");
	tc.Add("The Beatles");
	tc.Add("Bob Dylan");
	tc.Add("Red Hot Chili Peppers");
	tc.Add("Coldplay");}

	{auto& tc = list.GetAdd("Nurturing/motherly");
	tc.Add("Johnny Cash");}

	{auto& tc = list.GetAdd("Dark/tormented");
	tc.Add("Joy Division");
	tc.Add("Nine Inch Nails");
	tc.Add("Elliott Smith");
	tc.Add("Nick Cave and the Bad Seeds");
	tc.Add("The Cure");}

	{auto& tc = list.GetAdd("Remorseful/regretful");
	tc.Add("Red Hot Chili Peppers");
	tc.Add("Kurt Cobain");
	tc.Add("The National");}

	{auto& tc = list.GetAdd("Bold/brave");
	tc.Add("The Offspring");
	tc.Add("Red Hot Chili Peppers");
	tc.Add("The Clash");
	tc.Add("Bruce Springsteen");}

	{auto& tc = list.GetAdd("Outcast/rebel");
	tc.Add("The Offspring");
	tc.Add("Nirvana");
	tc.Add("The Smiths");
	tc.Add("Eminem");
	tc.Add("Green Day");
	tc.Add("Red Hot Chili Peppers");}

	{auto& tc = list.GetAdd("Lost/disconnected");
	tc.Add("Radiohead");
	tc.Add("Pink Floyd");
	tc.Add("Nine Inch Nails");
	tc.Add("The National");
	tc.Add("Red Hot Chili Peppers");}

	{auto& tc = list.GetAdd("Tough/badass");
	tc.Add("AC/DC");
	tc.Add("Metallica");
	tc.Add("Guns N' Roses");
	tc.Add("Led Zeppelin");
	tc.Add("The Rolling Stones");
	tc.Add("The Offspring");
	tc.Add("Red Hot Chili Peppers");
	}

	{auto& tc = list.GetAdd("Sincere/genuine");
	tc.Add("John Mayer");
	tc.Add("Red Hot Chili Peppers");
	tc.Add("Fleetwood Mac");
	tc.Add("The Offspring");
	tc.Add("James Taylor");}

	{auto& tc = list.GetAdd("Honest/vulnerable");
	tc.Add("Red Hot Chili Peppers");
	tc.Add("Ryan Adams");
	tc.Add("Sufjan Stevens");
	tc.Add("Phoebe Bridgers");}

	{auto& tc = list.GetAdd("Innocent/naive");
	tc.Add("The Beach Boys");
	tc.Add("Simon & Garfunkel");
	tc.Add("Red Hot Chili Peppers");
	tc.Add("The Beatles");}

	{auto& tc = list.GetAdd("Bold/risk-taking");
	tc.Add("Red Hot Chili Peppers");
	tc.Add("The Offspring");
	tc.Add("Nirvana");}
	
	ASSERT(list.GetCount() == GetTypecastCount());
	return list;
}

VectorMap<String,Vector<String>>& GetTypecastRappersMale() {
	thread_local static VectorMap<String,Vector<String>> list;
	if (!list.IsEmpty()) return list;
	
	{auto& tc = list.Add("Heartbroken/lovesick");
	tc.Add("Tupac Shakur");
	tc.Add("Eminem");
	tc.Add("Drake");
	tc.Add("Jay-Z");
	tc.Add("Public Enemy");}

	{auto& tc = list.Add("Rebel/anti-establishment");
	tc.Add("Run-DMC");
	tc.Add("N.W.A");
	tc.Add("Beastie Boys");
	tc.Add("Public Enemy");
	tc.Add("Rage Against the Machine");}

	{auto& tc = list.Add("Political activist");
	tc.Add("Tupac Shakur");
	tc.Add("Public Enemy");
	tc.Add("Rage Against the Machine");
	tc.Add("Jay-Z");
	tc.Add("N.W.A");}

	{auto& tc = list.Add("Social justice advocate");
	tc.Add("Public Enemy");
	tc.Add("Run-DMC");
	tc.Add("N.W.A");
	tc.Add("Jay-Z");
	tc.Add("Grandmaster Flash and the Furious Five");}

	{auto& tc = list.Add("Party/club");
	tc.Add("The Beastie Boys");
	tc.Add("Run-DMC");
	tc.Add("Limp Bizkit");
	tc.Add("Jay-Z");
	tc.Add("Drake");}

	{auto& tc = list.Add("Hopeful/dreamer");
	tc.Add("Tupac Shakur");
	tc.Add("Eminem");
	tc.Add("Jay-Z");
	tc.Add("Public Enemy");
	tc.Add("Drake");}

	{auto& tc = list.Add("Confident/empowered");
	tc.Add("Jay-Z");
	tc.Add("Eminem");
	tc.Add("Tupac Shakur");
	tc.Add("Drake");
	tc.Add("N.W.A");}

	{auto& tc = list.Add("Vulnerable/raw");
	tc.Add("Tupac Shakur");
	tc.Add("Eminem");
	tc.Add("Jay-Z");
	tc.Add("Public Enemy");
	tc.Add("Drake");}

	{auto& tc = list.Add("Romantic/love-driven");
	tc.Add("Drake");
	tc.Add("Tupac Shakur");
	tc.Add("Eminem");
	tc.Add("Jay-Z");
	tc.Add("N.W.A");}

	{auto& tc = list.Add("Failure/loser");
	tc.Add("Eminem");
	tc.Add("Tupac Shakur");
	tc.Add("N.W.A");
	tc.Add("Public Enemy");
	tc.Add("Drake");}

	{auto& tc = list.Add("Spiritual/faithful");
	tc.Add("Tupac Shakur");
	tc.Add("N.W.A");
	tc.Add("Public Enemy");
	tc.Add("Limp Bizkit");
	tc.Add("Jay-Z");}

	{auto& tc = list.Add("Passionate/determined");
	tc.Add("Eminem");
	tc.Add("Tupac Shakur");
	tc.Add("Jay-Z");
	tc.Add("N.W.A");
	tc.Add("Drake");}

	{auto& tc = list.Add("Reflective/self-reflective");
	tc.Add("Tupac Shakur");
	tc.Add("Eminem");
	tc.Add("Jay-Z");
	tc.Add("Public Enemy");
	tc.Add("DrakeConfident/self-assured");}

	{auto& tc = list.Add("Witty/sarcastic");
	tc.Add("Eminem");
	tc.Add("Beastie Boys");
	tc.Add("Tupac Shakur");
	tc.Add("Jay-Z");
	tc.Add("Public Enemy");}

	{auto& tc = list.Add("Melancholic/sad");
	tc.Add("Tupac Shakur");
	tc.Add("Eminem");
	tc.Add("Jay-Z");
	tc.Add("Drake");
	tc.Add("Limp Bizkit");}

	{auto& tc = list.Add("Humble/down-to-earth");
	tc.Add("Tupac Shakur");
	tc.Add("Eminem");
	tc.Add("Jay-Z");
	tc.Add("Drake");
	tc.Add("Public Enemy");}

	{auto& tc = list.Add("Charismatic/charming");
	tc.Add("Jay-Z");
	tc.Add("Tupac Shakur");
	tc.Add("Drake");
	tc.Add("Eminem");
	tc.Add("N.W.A");}

	{auto& tc = list.Add("Resilient/overcoming adversity");
	tc.Add("Tupac Shakur");
	tc.Add("Eminem");
	tc.Add("Jay-Z");
	tc.Add("Public Enemy");
	tc.Add("N.W.A");}

	{auto& tc = list.Add("Carefree/joyful");
	tc.Add("Jay-Z");
	tc.Add("Drake");
	tc.Add("N.W.A");
	tc.Add("Public Enemy");
	tc.Add("Limp Bizkit");}

	{auto& tc = list.Add("Dark/mysterious");
	tc.Add("Tupac Shakur");
	tc.Add("Eminem");
	tc.Add("Jay-Z");
	tc.Add("Limp Bizkit");
	tc.Add("N.W.A");}

	{auto& tc = list.Add("Comical/humorous");
	tc.Add("Eminem");
	tc.Add("Jay-Z");
	tc.Add("Beastie Boys");
	tc.Add("Run-DMC");
	tc.Add("N.W.A");}

	{auto& tc = list.Add("Controversial/provocative");
	tc.Add("Eminem");
	tc.Add("Tupac Shakur");
	tc.Add("N.W.A");
	tc.Add("Jay-Z");
	tc.Add("Public Enemy");}

	{auto& tc = list.Add("Nostalgic/sentimental");
	tc.Add("Tupac Shakur");
	tc.Add("Eminem");
	tc.Add("Limp Bizkit");
	tc.Add("Jay-Z");
	tc.Add("Drake");}

	{auto& tc = list.Add("Wise/philosophical");
	tc.Add("Tupac Shakur");
	tc.Add("Eminem");
	tc.Add("Jay-Z");
	tc.Add("Public Enemy");
	tc.Add("N.W.A");}

	{auto& tc = list.Add("Angry/outspoken");
	tc.Add("Rage Against the Machine");
	tc.Add("N.W.A");
	tc.Add("Public Enemy");
	tc.Add("Run-DMC");
	tc.Add("Eminem");}

	{auto& tc = list.Add("Calm/peaceful");
	tc.Add("Tupac Shakur");
	tc.Add("Eminem");
	tc.Add("Jay-Z");
	tc.Add("Limp Bizkit");
	tc.Add("N.W.A");}

	{auto& tc = list.Add("Confident/self-assured");
	tc.Add("Jay-Z");
	tc.Add("Drake");
	tc.Add("Eminem");
	tc.Add("Tupac Shakur");
	tc.Add("N.W.A");}

	{auto& tc = list.Add("Self-destructive/self-sabotaging");
	tc.Add("Eminem");
	tc.Add("Tupac Shakur");
	tc.Add("Limp Bizkit");
	tc.Add("Jay-Z");
	tc.Add("N.W.A");}

	{auto& tc = list.Add("Hopeful/optimistic");
	tc.Add("Tupac Shakur");
	tc.Add("Eminem");
	tc.Add("Limp Bizkit");
	tc.Add("Jay-Z");
	tc.Add("Drake");}

	{auto& tc = list.Add("Fearful/anxious");
	tc.Add("Tupac Shakur");
	tc.Add("Eminem");
	tc.Add("Drake");
	tc.Add("N.W.A");
	tc.Add("Public Enemy");}

	{auto& tc = list.Add("Eccentric/quirky");
	tc.Add("Beastie Boys");
	tc.Add("Limp Bizkit");
	tc.Add("Eminem");
	tc.Add("Jay-Z");
	tc.Add("N.W.A");}

	{auto& tc = list.Add("Sensitive/emotional");
	tc.Add("Eminem");
	tc.Add("Tupac Shakur");
	tc.Add("Jay-Z");
	tc.Add("N.W.A");
	tc.Add("Limp Bizkit");}

	{auto& tc = list.Add("Bitter/resentful");
	tc.Add("Eminem");
	tc.Add("Tupac Shakur");
	tc.Add("N.W.A");
	tc.Add("Public Enemy");
	tc.Add("Limp Bizkit");}

	{auto& tc = list.Add("Unique/nonconformist");
	tc.Add("Beastie Boys");
	tc.Add("Limp Bizkit");
	tc.Add("Public Enemy");
	tc.Add("N.W.A");
	tc.Add("Run-DMC");}

	{auto& tc = list.Add("Free-spirited/nonconformist");
	tc.Add("Tupac Shakur");
	tc.Add("Eminem");
	tc.Add("N.W.A");
	tc.Add("Public Enemy");
	tc.Add("Run-DMC");}

	{auto& tc = list.Add("Sultry/seductive");
	tc.Add("Drake");
	tc.Add("Jay-Z");
	tc.Add("Eminem");
	tc.Add("Tupac Shakur");
	tc.Add("N.W.A");}

	{auto& tc = list.Add("Inspirational/motivational");
	tc.Add("Tupac Shakur");
	tc.Add("Eminem");
	tc.Add("Jay-Z");
	tc.Add("Public Enemy");
	tc.Add("Drake");}

	{auto& tc = list.Add("Authentic/real");
	tc.Add("Tupac Shakur");
	tc.Add("Eminem");
	tc.Add("Jay-Z");
	tc.Add("Public Enemy");
	tc.Add("N.W.A");}

	{auto& tc = list.Add("Mysterious/enigmatic");
	tc.Add("Tupac Shakur");
	tc.Add("Eminem");
	tc.Add("Jay-Z");
	tc.Add("Limp Bizkit");
	tc.Add("N.W.A");}

	{auto& tc = list.Add("Carefree/bohemian");
	tc.Add("Jay-Z");
	tc.Add("Drake");
	tc.Add("N.W.A");
	tc.Add("Run-DMC");
	tc.Add("Eminem");}

	{auto& tc = list.Add("Street-smart/tough");
	tc.Add("Tupac Shakur");
	tc.Add("Eminem");
	tc.Add("N.W.A");
	tc.Add("Public Enemy");
	tc.Add("Limp Bizkit");}

	{auto& tc = list.Add("Romantic/idealistic");
	tc.Add("Tupac Shakur");
	tc.Add("Eminem");
	tc.Add("Jay-Z");
	tc.Add("Public Enemy");
	tc.Add("Drake");}

	{auto& tc = list.Add("Nurturing/motherly");
	tc.Add("Tupac Shakur");
	tc.Add("Eminem");
	tc.Add("Jay-Z");
	tc.Add("N.W.A");
	tc.Add("Public Enemy");}

	{auto& tc = list.Add("Dark/tormented");
	tc.Add("Tupac Shakur");
	tc.Add("Eminem");
	tc.Add("Jay-Z");
	tc.Add("Public Enemy");
	tc.Add("N.W.A");}

	{auto& tc = list.Add("Remorseful/regretful");
	tc.Add("Tupac Shakur");
	tc.Add("Eminem");
	tc.Add("Jay-Z");
	tc.Add("Public Enemy");
	tc.Add("Drake");}

	{auto& tc = list.Add("Bold/brave");
	tc.Add("Tupac Shakur");
	tc.Add("Eminem");
	tc.Add("Jay-Z");
	tc.Add("N.W.A");
	tc.Add("Public Enemy");}

	{auto& tc = list.Add("Outcast/rebel");
	tc.Add("Tupac Shakur");
	tc.Add("N.W.A");
	tc.Add("Jay-Z");
	tc.Add("Eminem");
	tc.Add("Public Enemy");}

	{auto& tc = list.Add("Lost/disconnected");
	tc.Add("Tupac Shakur");
	tc.Add("Jay-Z");
	tc.Add("Drake");
	tc.Add("Limp Bizkit");
	tc.Add("N.W.A");}

	{auto& tc = list.Add("Tough/badass");
	tc.Add("Jay-Z");
	tc.Add("N.W.A");
	tc.Add("Eminem");
	tc.Add("Tupac Shakur");
	tc.Add("Public Enemy");}

	{auto& tc = list.Add("Sincere/genuine");
	tc.Add("Tupac Shakur");
	tc.Add("Eminem");
	tc.Add("N.W.A");
	tc.Add("Public Enemy");
	tc.Add("Drake");}

	{auto& tc = list.Add("Honest/vulnerable");
	tc.Add("Tupac Shakur");
	tc.Add("Eminem");
	tc.Add("N.W.A");
	tc.Add("Public Enemy");
	tc.Add("Jay-Z");}

	{auto& tc = list.Add("Innocent/naive");
	tc.Add("Tupac Shakur");
	tc.Add("Eminem");
	tc.Add("Jay-Z");
	tc.Add("Public Enemy");
	tc.Add("N.W.A");}

	{auto& tc = list.Add("Bold/risk-taking");
	tc.Add("Tupac Shakur");
	tc.Add("Eminem");
	tc.Add("Jay-Z");
	tc.Add("N.W.A");
	tc.Add("Public Enemy");}
	
	ASSERT(list.GetCount() == GetTypecastCount());
	return list;
}


END_TEXTLIB_NAMESPACE

