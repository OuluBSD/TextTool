#include "ToolCore.h"


VectorMap<String,Vector<String>>& GetTypecastArtists() {
	thread_local static VectorMap<String,Vector<String>> list;
	if (!list.IsEmpty()) return list;
	
	{auto& tc = list.Add("Heartbroken/lovesick");
	tc.Add("Adele");
	tc.Add("Taylor Swift");
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
	tc.Add("Katy Perry");
	tc.Add("Coldplay");
	tc.Add("U2");
	tc.Add("Imagine Dragons");
	tc.Add("Miley Cyrus");
	tc.Add("Beyonce");}

	{auto& tc = list.Add("Confident/empowered");
	tc.Add("Beyonce");
	tc.Add("Whitney Houston");
	tc.Add("Destiny's Child");
	tc.Add("Lady Gaga");
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
	tc.Add("Taylor Swift");
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
	tc.Add("Lady Gaga");
	tc.Add("Kanye West");
	tc.Add("Pink");
	tc.Add("Bruce Springsteen");
	tc.Add("Shakira");
	tc.Add("Katy Perry");
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
	tc.Add("Taylor Swift (1989 album)");
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
	tc.Add("Bob Dylan (protest songs)");}

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
	tc.Add("Taylor Swift (1989 album)");
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
	tc.Add("Taylor Swift (Speak Now album)");
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
	tc.Add("Lady Gaga");
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
	tc.Add("Taylor Swift");
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
	tc.Add("Taylor Swift");
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
	tc.Add("Lady Gaga");
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
	tc.Add("Taylor Swift");
	tc.Add("Adele");
	tc.Add("Ed Sheeran");
	tc.Add("Billie Eilish");
	tc.Add("Lorde");
	tc.Add("Hozier");
	tc.Add("Lana Del Rey");
	tc.Add("James Bay");
	tc.Add("Shawn Mendes");}

	{auto& tc = list.Add("Innocent/naive");
	tc.Add("Taylor Swift");
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
	tc.Add("Lady Gaga");
	tc.Add("Beyonce");
	tc.Add("Pink Floyd (early years)");
	tc.Add("Patti Smith");
	tc.Add("The Rolling Stones");
	tc.Add("Queen");
	}
	return list;
}