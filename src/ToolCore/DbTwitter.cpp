#include "ToolCore.h"
#include "ProtectedCommon.h"

BEGIN_TEXTLIB_NAMESPACE

const Index<String>& GetRoles() {
	static Index<String> list;
	if (list.IsEmpty()) {
		list.Add("Influencer");
		list.Add("Activist");
		list.Add("Expert");
		list.Add("Comedian");
		list.Add("Politician");
		list.Add("Social media personality");
		list.Add("Marketer");
		list.Add("Journalist");
		list.Add("Writer/author");
		list.Add("Celebrity/entertainer");
		list.Add("Blogger");
		list.Add("Entrepreneur");
		list.Add("Educator");
		list.Add("Student");
		list.Add("Parent");
		list.Add("Athlete");
		list.Add("Music fan");
		list.Add("Foodie");
		list.Add("Traveler");
		list.Add("Developer/programmer");
		list.Add("Entity/creative");
		list.Add("Scientist/researcher");
		list.Add("Environmentalist");
		list.Add("Animal lover/activist");
		list.Add("Fashionista");
		list.Add("Homemaker");
		list.Add("Philanthropist");
		list.Add("Socialite");
		list.Add("Food/drink critic");
		list.Add("Gamer");
		list.Add("Fitness enthusiast");
		list.Add("Health/wellness guru");
		list.Add("Spiritual leader");
		list.Add("Parenting advice");
		list.Add("Career coach/advisor");
		list.Add("Travel blogger");
		list.Add("Book lover/reader");
		list.Add("DIY enthusiast");
		list.Add("Pet lover/owner");
		list.Add("Movie/TV critic");
		list.Add("Beauty/fashion blogger");
		list.Add("Tech geek");
		list.Add("Nature lover");
		list.Add("Political commentator");
		list.Add("Relationship expert");
		list.Add("Human rights activist");
		list.Add("Social justice warrior");
		list.Add("Music reviewer");
		list.Add("Interior design enthusiast");
		list.Add("Self-help guru");
		list.Add("Life coach");
		list.Add("Mental health advocate");
		list.Add("Promoter/event organizer");
		list.Add("Financial advisor");
		list.Add("Food blogger");
		list.Add("Sports enthusiast");
		list.Add("Fashion designer");
		list.Add("Makeup artist");
		list.Add("Gardening enthusiast");
		list.Add("Geek/nerd");
		list.Add("History buff");
		list.Add("Business owner");
		list.Add("Legal expert");
		list.Add("Parenting blogger");
		list.Add("Senior citizen/retiree");
		list.Add("Marriage counselor");
		list.Add("Wine connoisseur");
		list.Add("Youth advocate");
		list.Add("Success coach");
		list.Add("Career woman/man");
		list.Add("Fitness coach");
		list.Add("Political blogger");
		list.Add("Blogger/influencer relations");
		list.Add("Adult entertainer");
		list.Add("Adult content creator");
		list.Add("Adult industry critic");
		list.Add("Adult content reviewer");
	}
	return list;
}

int GetRoleCount() {
	return GetRoles().GetCount();
}

const Vector<ContentType>& GetGenerics() {
	static Vector<ContentType> list;
	if (list.IsEmpty()) {
		list.Add().Set("Rise to fame", "a person shares their journey and successes", "shares achievements and milestones", "shares their expertise and advice for others to achieve success");
		list.Add().Set("Call to Action", "person speaks out on important social or political issues", "uses their platform and influence to promote change", "urges others to take action and make a difference");
		list.Add().Set("Everyday life updates", "person shares personal and relatable experiences", "offers glimpses into their daily routines", "shares thoughts and opinions on current events or pop culture");
		list.Add().Set("Brand promotion", "person promotes their products or services", "creates hype and anticipation for upcoming snaps or events", "collaborates with other brands and influencers to expand reach and exposure");
		list.Add().Set("Entertainment", "person shares jokes and comedic content", "reacts to memes and trending topics", "creates funny and entertaining videos/performances");
		list.Add().Set("Advocacy and awareness", "person raises awareness for important causes", "shares personal stories and experiences", "educates and informs their followers on important issues");
		list.Add().Set("Political opinions and debates", "person shares their political views and beliefs", "engages in debates and discussions with others", "campaigns for a particular candidate or party");
		list.Add().Set("Behind the scenes", "person offers an inside look into their creative process or work", "shares sneak peeks of upcoming projects", "takes followers on a virtual tour of their workspace");
		list.Add().Set("Travel adventures", "person shares photos and videos from their travels", "explores new places and cultures", "shares tips and recommendations for others interested in traveling");
		list.Add().Set("Fitness journey", "person shares their fitness goals and progress", "creates workout videos and tutorials", "motivates and inspires others to prioritize their health and fitness");
		list.Add().Set("Food and cooking", "person shares their favorite recipes and cooking tips", "reviews restaurants and food products", "documents their food adventures and cooking experiments");
		list.Add().Set("Tech and innovation", "person shares news and updates on the latest technology and innovations", "offers tech advice and recommendations", "discusses the impact of technology on society");
		list.Add().Set("Self-care and wellness", "person promotes self-care practices and mindfulness", "shares motivational quotes and tips for self-improvement", "discusses mental health and self-care");
		list.Add().Set("Relationships and love", "person shares their own experiences with relationships and dating", "offers advice and support to others going through similar situations", "discusses different types of relationships and love");
		list.Add().Set("Fashion and style", "person showcases their own fashion and style", "collaborates with fashion brands and influencers for sponsored content", "offers fashion tips and advice");
		list.Add().Set("Music and concerts", "person shares their favorite music and entities", "attends and covers concerts and music festivals", "discusses the impact of music on culture and society");
		list.Add().Set("DIY and crafts", "person shares DIY tutorials and projects", "offers tips and tricks for crafting and home decor", "encourages others to unleash their creativity");
		list.Add().Set("Animal lover", "person shares photos and videos of their pets", "raises awareness for animal rights and welfare", "promotes adoption and rescue organizations");
		list.Add().Set("Reviews and recommendations", "person shares their thoughts and opinions on products, services, and experiences", "provides honest reviews and recommendations", "collaborates with brands for sponsored reviews");
		list.Add().Set("Beauty and makeup", "person shares makeup tutorials and beauty tips", "collaborates with beauty brands for sponsored content", "discusses body positivity and self-love");
		list.Add().Set("Bookworm", "person shares their current reads and book recommendations", "participates in book clubs and discussions", "writes book reviews and author interviews");
		list.Add().Set("Gaming and esports", "person shares their favorite games and gaming setup", "streams their gameplay for followers", "discusses the latest news and trends in the gaming industry");
		list.Add().Set("Education and learning", "person shares their educational journey and tips for academic success", "creates educational content and resources", "discusses the importance of education and lifelong learning");
		list.Add().Set("Nature and conservation", "person shares photos and videos of nature and wildlife", "raises awareness for environmental conservation and sustainability", "discusses ways to protect and preserve the planet");
		list.Add().Set("Entrepreneurship", "person shares their experience and lessons as a business owner", "offers business advice and strategies", "collaborates with other entrepreneurs and businesses for networking and growth");
		list.Add().Set("Art and creativity", "person showcases their artistic talents and creations", "collaborates with other entities and galleries for exposure", "discusses the impact of art on society");
		list.Add().Set("Health and medical advice", "person shares medical advice and resources", "discusses the latest news and research in the healthcare industry", "raises awareness for health issues and campaigns");
		list.Add().Set("Celebrity gossip", "person shares the latest celebrity news and rumors", "participates in discussions and debates about celebrities and their personal lives", "creates humorous content and memes related to celebrity culture");
		list.Add().Set("Music producer and songwriter", "person shares their own original music and songwriting process", "collaborates with other entities and producers for music projects", "shares behind the scenes footage of music production and studio work");
		list.Add().Set("Influencer on the rise", "person documents their journey as an up-and-coming social media influencer", "shares tips and strategies for growing a following and establishing a brand on social media", "collaborates with brands for sponsored content and partnerships");
		/*list.Add().Set("Wedding planning", "person shares their own wedding planning journey and tips", "offers wedding planning services and advice for followers", "collaborates with wedding vendors and venues");
		list.Add().Set("Political news junkie", "person shares news articles and updates on politics and current events", "discusses and analyses political policies and decisions", "participates in online discussions and debates with others");
		list.Add().Set("Philanthropy and charity", "person raises money and awareness for charitable causes", "participates in fundraising events and campaigns", "uses their platform for good and to make a positive impact on society");
		list.Add().Set("Career and job advice", "person shares their career journey and tips for professional growth", "offers job search strategies and resume advice", "discusses workplace culture and trends");
		list.Add().Set("Senior citizen lifestyle", "person shares their experiences and struggles as a senior citizen", "discusses issues related to aging and retirement", "offers advice and support to other seniors");
		list.Add().Set("Travel photography", "person captures and shares stunning photos of different locations around the world", "offers photography tips and advice", "collaborates with travel brands for sponsored content");
		list.Add().Set("Culinary adventures", "person shares their culinary experiences and food adventures", "tries new and unique foods and cuisines", "discovers hidden food gems in their city");
		list.Add().Set("LGBTQ+ advocate", "person shares their personal experiences and insights as a member of the LGBTQ+ community", "raises awareness for LGBTQ+ rights and issues", "participates in Pride events and campaigns");
		list.Add().Set("Motivational speaker", "person shares inspirational quotes and words of wisdom", "offers advice and support to followers", "shares their own personal journey and obstacles overcome");
		list.Add().Set("News and current events", "person discusses and provides updates on important news and current events", "engages in discussions and debates on controversial topics", "shares unbiased and factual information");
		list.Add().Set("Fitness challenges", "person creates fitness challenges and workouts for followers to join", "tracks their progress and invites others to do the same", "offers prizes and incentives for completion");
		list.Add().Set("Hiking and outdoors", "person shares photos and videos of their outdoor adventures", "offers tips and recommendations for hiking and camping", "advocates for environmental conservation and protection");
		list.Add().Set("Parenting humor", "person shares funny and relatable parenting memes and content", "creates humorous videos and sketches about the realities of parenting", "engages in lighthearted discussions and debates with other parents");
		list.Add().Set("Financial literacy", "person shares budgeting tips and financial advice", "discusses the importance of saving and investing", "collaborates with financial advisors and experts for educational content");
		list.Add().Set("Pop culture analysis", "person shares their thoughts and opinions on trends and pop culture phenomena", "discusses the impact of popular media on society", "creates content that deconstructs and analyzes popular culture");
		list.Add().Set("Mental health advocacy", "person shares their personal struggles and journey with mental health", "raises awareness and fights stigma associated with mental illness", "offers resources and support to those struggling with mental health issues");
		list.Add().Set("Raising a special needs child", "person shares their experiences and challenges raising a child with special needs", "offers support and resources to other parents in similar situations", "advocates for improved rights and accommodations for individuals with disabilities");
		list.Add().Set("Marketing and branding strategies", "person shares their expertise in marketing and branding", "offers tips and advice for entrepreneurs and small business owners", "collaborates with brands for sponsored content and partnerships");
		list.Add().Set("Conspiracy theories", "person shares theories and evidence supporting various conspiracy theories", "participates in discussions and debates with other believers and skeptics", "creates content that dives deep into the world of conspiracy");
		list.Add().Set("Tech gadgets and reviews", "person shares reviews and recommendations for the latest tech gadgets and devices", "offers tech advice and tutorials", "collaborates with tech brands for sponsored content and reviews");
		list.Add().Set("The single life", "person shares their experiences and thoughts on being single", "discusses dating and relationships", "creates content that challenges the societal norms and expectations surrounding being in a relationship");
		list.Add().Set("Online activism", "person uses their platform and influence to advocate for social and political issues", "participates in online campaigns and hashtags", "encourages their followers to take action and make a difference");
		list.Add().Set("Music producer", "person shares their music production process and techniques", "offers tips and tricks for aspiring producers", "collaborates with other entities for music projects");
		list.Add().Set("Singer in a rock band", "person shares music from their band and performances at concerts and festivals", "creates music videos and behind the scenes footage", "discusses the rock music genre and its evolution");
		list.Add().Set("Running and fitness challenges", "person participates in running challenges and documents their progress", "encourages followers to join them in achieving fitness goals", "advocates for running as a form of physical and mental health");
		list.Add().Set("Rapper", "person shares their original rap components and music videos", "documents the process of creating and producing rap music", "discusses the hip hop and rap culture");
		list.Add().Set("Rap music producer", "person shares their production techniques and collaborations with rap entities", "discusses the evolution and trends in rap music production", "offers advice and resources for aspiring rap music producers");
		list.Add().Set("Luxury lifestyle", "person shares their luxurious travels, fashion, and lifestyle", "collaborates with luxury brands for sponsored content", "creates content that showcases and celebrates the finer things in life");
		list.Add().Set("Being a new parent", "person shares the ups and downs of being a new parent", "offers advice and support for other new parents", "documents their child's growth and milestones");
		list.Add().Set("Cheap local traveller", "person documents their budget-friendly travels to local destinations", "offers tips and recommendations for affordable travel", "advocates for responsible and sustainable tourism");
		list.Add().Set("Bakery owner", "person shares their baking journey and creates content featuring their bakery", "offers baking tips and recipes", "promotes their bakery and engages with customers online");
		list.Add().Set("Music photographer", "person shares their concert photography and behind the scenes shots", "offers photography tips and advice", "collaborates with music entities and festivals for content and coverage");
		list.Add().Set("Metal music producer", "person shares their process of producing metal music and collaborations with metal entities", "discusses the history and sub-genres of metal music", "offers resources and advice for aspiring metal music producers");
		list.Add().Set("Self-taught artist", "person documents their art journey and progress as a self-taught artist", "shares tutorials and tips for aspiring entities", "collaborates with other entities for inspiration and growth");
		list.Add().Set("Beet enthusiasist", "person shares their love for beets and creates original recipes and dishes featuring beets", "discusses the health benefits and uses of beets", "participates in online discussions with other beet lovers");
		list.Add().Set("Rock music producer", "person shares their process of producing rock music and collaborations with rock entities", "discusses the evolution and trends of rock music production", "offers resources and advice for aspiring rock music producers");
		list.Add().Set("News commentator", "person shares their perspectives and analysis on current events and news stories", "discusses politics, society, and culture", "participates in debates and discussions with others");
		list.Add().Set("Left-wing commentator", "person shares their liberal views and critiques on politics and social issues", "offers alternative perspectives and solutions to current problems", "advocates for progressive change and activism");
		list.Add().Set("Music business owner", "person shares their experiences and challenges as a music business owner", "offers advice and resources for running a successful music business", "collaborates with other professionals in the music industry");
		list.Add().Set("Cheap local hiker", "person documents their budget-friendly hikes and nature adventures in their local area", "offers tips and recommendations for affordable outdoor activities", "advocates for environmental conservation and responsible hiking");
		list.Add().Set("Downhill skiing enthusiast", "person shares footage and photos from their downhill skiing adventures", "discusses techniques and gear for skiing", "collaborates with ski resorts and brands for sponsored content");
		list.Add().Set("Kayak owner", "person shares their kayaking trips and adventures", "offers tips and recommendations for beginner kayakers", "discusses the benefits and environmental impact of kayaking");
		list.Add().Set("Podcaster", "person hosts and produces their own podcast on a specific topic or theme", "invites guests to share their expertise and opinions", "promotes and engages with listeners on social media");
		list.Add().Set("Music podcaster", "person hosts and produces a podcast featuring interviews and discussions with music entities and industry professionals", "discusses the latest trends and happenings in the music industry", "collaborates with independent musicians and record labels for content");
		list.Add().Set("Music production podcaster", "person hosts and produces a podcast on the topic of music production", "features interviews and discussions with music producers and engineers", "offers tips and resources for aspiring music producers");
		list.Add().Set("Food vlogger", "person creates videos documenting their culinary adventures and recipes", "collaborates with restaurants and brands for sponsored content", "offers cooking and food-related tips and tutorials");
		list.Add().Set("Adventure seeker", "person shares their adrenaline-fueled travels and activities", "documents extreme sports and outdoor adventures", "advocates for embracing new challenges and getting out of one's comfort zone");
		list.Add().Set("Metal band vlogger", "person documents their life and career as a member of a metal band", "shares videos of live performances and behind the scenes footage", "discusses the metal music community and culture");
		list.Add().Set("Fitness model", "person shares their fitness journey and offers workout and nutrition advice", "collaborates with fitness and sports brands for sponsored content", "promotes body positivity and healthy living");
		list.Add().Set("EDM producer vlogger", "person documents their journey and process of producing electronic dance music", "shares behind the scenes footage of festivals and events", "collaborates with other EDM entities and DJs for content");
		list.Add().Set("C++ programmer", "person shares their coding projects and programming tips and resources for C++", "discusses advancements and updates in C++ programming language", "participates in online discussions and collaborations with fellow C++ programmers");
		list.Add().Set("Small business owner", "person shares their experiences and challenges of owning a small business", "discusses entrepreneurship and offers advice for other small business owners", "collaborates with other entrepreneurs and local businesses for a stronger community");
		list.Add().Set("Daily life vlogger", "person shares snippets of their daily life and activities", "creates a personal connection with followers by providing a glimpse into their life", "engages with followers and responds to comments and questions");
		list.Add().Set("Beauty guru", "person creates makeup tutorials, product reviews, and beauty hacks", "collaborates with beauty brands for sponsored content", "promotes self-love and confidence through makeup");
		list.Add().Set("Animal rescue advocate", "person shares their experiences rescuing and caring for animals in need", "promotes adoption and responsible pet ownership", "participates in online campaigns and donations for animal rescue organizations");
		list.Add().Set("Food critic", "person shares reviews and critiques of restaurants and dishes", "creates food-related content such as recipes and food challenges", "collaborates with restaurants and food brands for sponsored content");
		list.Add().Set("Travel photographer", "person shares their travel photography from destinations around the world", "offers tips for improving photography skills and capturing unique shots", "collaborates with tourism boards and travel brands for sponsored content");
		list.Add().Set("Music business thoughts", "person shares their opinions and ideas on the music industry and its future", "discusses the impact of technology and social media on the music business", "collaborates with other industry professionals for discussions and debates");
		list.Add().Set("Thoughts about the music business" , "person shares their experiences and insights as a music industry professional", "offers advice and resources for aspiring musicians and industry professionals", "discusses the challenges and opportunities in the music business");
		list.Add().Set("Thoughts about other entities as an music artist", "person shares their thoughts and analysis on other music entities and their work", "engages in discussions and debates about different music genres and styles", "creates content that promotes and supports other talent");
		list.Add().Set("Starting a business from scratch", "person documents their journey and struggles of starting a business from the ground up", "offers advice and resources for aspiring entrepreneurs", "encourages and motivates others to pursue their dreams");
		list.Add().Set("Art collector", "person shares their art collection and discusses the stories behind each piece", "offers tips for building an art collection and investing in art", "collaborates with entities and galleries for sponsored content and events");
		list.Add().Set("Music listener", "person shares their music playlist and recommendations for different music genres", "engages in discussions and debates about music and entities", "creates content that celebrates the power and influence of music");
		list.Add().Set("Metal music listener", "person shares their metal music playlist and attends concerts and festivals", "participates in online discussions and debates about metal music", "creates content that promotes and celebrates the metal music community");
		*/
	}
	return list;
}

int GetGenericCount() {
	return GetGenerics().GetCount();
}


VectorMap<String,Vector<String>>& GetRolesSafeMale() {
	thread_local static VectorMap<String,Vector<String>> list;
	if (!list.IsEmpty()) return list;
	
	{auto& tc = list.Add("Influencer");
	tc.Add("Barack Obama");
	tc.Add("Dwayne \"The Rock\" Johnson");
	tc.Add("Justin Timberlake");
	tc.Add("Will Smith");
	tc.Add("Neil Patrick Harris");}

	{auto& tc = list.Add("Activist");
	tc.Add("Malala Yousafzai");
	tc.Add("Colin Kaepernick");
	tc.Add("Leonardo DiCaprio");
	tc.Add("Emma Watson");
	tc.Add("George Takei");}

	{auto& tc = list.Add("Expert");
	tc.Add("Dr. Fauci");
	tc.Add("Bill Gates");
	tc.Add("Neil deGrasse Tyson");
	tc.Add("Anderson Cooper");
	tc.Add("Trevor Noah");}

	{auto& tc = list.Add("Comedian");
	tc.Add("John Mulaney");
	tc.Add("Hasan Minhaj");
	tc.Add("Steve Carell");
	tc.Add("Andy Samberg");
	tc.Add("Jerry Seinfeld");}

	{auto& tc = list.Add("Politician");
	tc.Add("Joe Biden");
	tc.Add("Bernie Sanders");
	tc.Add("Justin Trudeau");
	tc.Add("Jacinda Ardern");
	tc.Add("Emmanuel Macron");}

	{auto& tc = list.Add("Social media personality");
	tc.Add("Ryan Seacrest");
	tc.Add("Felix Kjellberg (PewDiePie)");
	tc.Add("MrBeast");
	tc.Add("David Dobrik");
	tc.Add("Jeffree Star");}

	{auto& tc = list.Add("Marketer");
	tc.Add("Gary Vaynerchuk");
	tc.Add("Seth Godin");
	tc.Add("Tim Ferriss");
	tc.Add("Neil Patel");
	tc.Add("Grant Cardone");}

	{auto& tc = list.Add("Journalist");
	tc.Add("Anderson Cooper");
	tc.Add("Lester Holt");
	tc.Add("Christiane Amanpour");
	tc.Add("Robin Roberts");
	tc.Add("Jake Tapper");}

	{auto& tc = list.Add("Writer/author");
	tc.Add("Stephen King");
	tc.Add("J.K. Rowling");
	tc.Add("John Green");
	tc.Add("James Patterson");
	tc.Add("Malcolm Gladwell");}

	{auto& tc = list.Add("Celebrity/entertainer");
	tc.Add("Tom Hanks");
	tc.Add("Chris Pratt");
	tc.Add("Lin-Manuel Miranda");
	tc.Add("Leonardo DiCaprio");
	tc.Add("Drake");}

	{auto& tc = list.Add("Blogger");
	tc.Add("Seth Godin");
	tc.Add("Tim Ferriss");
	tc.Add("Neil Patel");
	tc.Add("Rand Fishkin");
	tc.Add("James Clear");}

	{auto& tc = list.Add("Entrepreneur");
	tc.Add("Elon Musk");
	tc.Add("Richard Branson");
	tc.Add("Mark Cuban");
	tc.Add("Daymond John");
	tc.Add("Peter Thiel");}

	{auto& tc = list.Add("Educator");
	tc.Add("Bill Nye");
	tc.Add("Neil deGrasse Tyson");
	tc.Add("Ken Robinson");
	tc.Add("Salman Khan");
	tc.Add("Sir David Attenborough");}

	{auto& tc = list.Add("Student");
	tc.Add("Greta Thunberg");
	tc.Add("Malala Yousafzai");
	tc.Add("Shawn Mendes");
	tc.Add("Zendaya");
	tc.Add("Millie Bobby Brown");}

	{auto& tc = list.Add("Parent");
	tc.Add("Barack Obama");
	tc.Add("Neil Patrick Harris");
	tc.Add("John Legend");
	tc.Add("Ashton Kutcher");
	tc.Add("Dwayne \"The Rock\" Johnson");}

	{auto& tc = list.Add("Athlete");
	tc.Add("LeBron James");
	tc.Add("Tom Brady");
	tc.Add("Cristiano Ronaldo");
	tc.Add("Lionel Messi");
	tc.Add("Serena Williams");}

	{auto& tc = list.Add("Music fan");
	tc.Add("Ed Sheeran");
	tc.Add("Harry Styles");
	tc.Add("Billie Eilish");
	tc.Add("Bruno Mars");
	tc.Add("Lizzo");}

	{auto& tc = list.Add("Foodie");
	tc.Add("Anthony Bourdain");
	tc.Add("Jamie Oliver");
	tc.Add("Rachael Ray");
	tc.Add("Gordon Ramsey");
	tc.Add("Samantha Lee");}

	{auto& tc = list.Add("Traveler");
	tc.Add("Anthony Bourdain");
	tc.Add("Rick Steves");
	tc.Add("Bear Grylls");
	tc.Add("Anthony Melchiorri");
	tc.Add("Andrew Zimmern");}

	{auto& tc = list.Add("Developer/programmer");
	tc.Add("Linus Torvalds");
	tc.Add("Mark Zuckerberg");
	tc.Add("Elon Musk");
	tc.Add("Tim Cook");
	tc.Add("Jack Dorsey");}

	{auto& tc = list.Add("Entity/creative");
	tc.Add("Walt Disney");
	tc.Add("J.K. Rowling");
	tc.Add("Stan Lee");
	tc.Add("George Lucas");
	tc.Add("Steven Spielberg");}

	{auto& tc = list.Add("Scientist/researcher");
	tc.Add("Neil deGrasse Tyson");
	tc.Add("Bill Nye");
	tc.Add("Stephen Hawking");
	tc.Add("Jane Goodall");
	tc.Add("Edward O. Wilson");}

	{auto& tc = list.Add("Environmentalist");
	tc.Add("Leonardo DiCaprio");
	tc.Add("Jane Goodall");
	tc.Add("Jeff Corwin");
	tc.Add("Ed Begley Jr.");
	tc.Add("David Attenborough");}

	{auto& tc = list.Add("Animal lover/activist");
	tc.Add("Ellen DeGeneres");
	tc.Add("Dwayne \"The Rock\" Johnson");
	tc.Add("Jessica Chastain");
	tc.Add("Ian Somerhalder");
	tc.Add("Mandy Moore");}

	{auto& tc = list.Add("Fashionista");
	tc.Add("Tim Gunn");
	tc.Add("Tan France");
	tc.Add("Ralph Lauren");
	tc.Add("Tom Ford");
	tc.Add("Alexander Wang");}

	{auto& tc = list.Add("Homemaker");
	tc.Add("Drew Barrymore");
	tc.Add("Joanna Gaines");
	tc.Add("Martha Stewart");
	tc.Add("Nate Berkus");
	tc.Add("Chef Jamie Oliver");}

	{auto& tc = list.Add("Philanthropist");
	tc.Add("Bill Gates");
	tc.Add("Warren Buffett");
	tc.Add("Melinda Gates");
	tc.Add("Elon Musk");
	tc.Add("Mark Zuckerberg");}

	{auto& tc = list.Add("Socialite");
	tc.Add("Paris Hilton");
	tc.Add("Kendall Jenner");
	tc.Add("Nicky Hilton");
	tc.Add("Ivanka Trump");
	tc.Add("Olivia Palermo");}

	{auto& tc = list.Add("Food/drink critic");
	tc.Add("Anthony Bourdain");
	tc.Add("Gordan Ramsey");
	tc.Add("Ruth Reichl");
	tc.Add("Guy Fieri");
	tc.Add("Andrew Zimmern");}

	{auto& tc = list.Add("Gamer");
	tc.Add("PewDiePie");
	tc.Add("Markiplier");
	tc.Add("DanTDM");
	tc.Add("Ninja");
	tc.Add("Shroud");}

	{auto& tc = list.Add("Fitness enthusiast");
	tc.Add("David Beckham");
	tc.Add("Usain Bolt");
	tc.Add("Jillian Michaels");
	tc.Add("Michael Phelps");
	tc.Add("Cristiano Ronaldo");}

	{auto& tc = list.Add("Health/wellness guru");
	tc.Add("Deepak Chopra");
	tc.Add("Dr. Oz");
	tc.Add("Mehmet Oz");
	tc.Add("Andrew Weil");
	tc.Add("Jillian Michaels");}

	{auto& tc = list.Add("Spiritual leader");
	tc.Add("Dalai Lama");
	tc.Add("Pope Francis");
	tc.Add("Joel Osteen");
	tc.Add("Thich Nhat Hanh");
	tc.Add("Deepak Chopra");}

	{auto& tc = list.Add("Parenting advice");
	tc.Add("Barack Obama");
	tc.Add("Ashton Kutcher");
	tc.Add("Chip Gaines");
	tc.Add("Kevin Jonas");
	tc.Add("Dwayne \"The Rock\" Johnson");}

	{auto& tc = list.Add("Career coach/advisor");
	tc.Add("Tony Robbins");
	tc.Add("Tim Ferriss");
	tc.Add("Marie Forleo");
	tc.Add("Gary Vaynerchuk");
	tc.Add("Mel Robbins");}

	{auto& tc = list.Add("Travel blogger");
	tc.Add("Chris Burkard");
	tc.Add("Matt Kepnes (Nomadic Matt)");
	tc.Add("Brooke Saward (World of Wanderlust)");
	tc.Add("Lauren Bullen (Gypsea Lust)");
	tc.Add("Kiersten Rich (The Blonde Abroad)");}

	{auto& tc = list.Add("Book lover/reader");
	tc.Add("Neil Gaiman");
	tc.Add("Stephen King");
	tc.Add("John Green");
	tc.Add("James Patterson");
	tc.Add("J.K. Rowling");}

	{auto& tc = list.Add("DIY enthusiast");
	tc.Add("Bob Vila");
	tc.Add("Ty Pennington");
	tc.Add("Chip Gaines");
	tc.Add("Tom Silva");
	tc.Add("Adam Savage");}

	{auto& tc = list.Add("Pet lover/owner");
	tc.Add("Ellen DeGeneres");
	tc.Add("Drew Barrymore");
	tc.Add("Mandy Moore");
	tc.Add("Chris Evans");
	tc.Add("Patrick Stewart");}

	{auto& tc = list.Add("Movie/TV critic");
	tc.Add("Roger Ebert");
	tc.Add("Leonard Maltin");
	tc.Add("Peter Travers");
	tc.Add("Gene Siskel");
	tc.Add("Richard Roeper");}

	{auto& tc = list.Add("Beauty/fashion blogger");
	tc.Add("Michelle Phan");
	tc.Add("Zoella");
	tc.Add("Huda Kattan");
	tc.Add("Rumi Neely");
	tc.Add("Tanya Burr");}

	{auto& tc = list.Add("Tech geek");
	tc.Add("Elon Musk");
	tc.Add("Bill Gates");
	tc.Add("Mark Zuckerberg");
	tc.Add("Jack Ma");
	tc.Add("Larry Page");}

	{auto& tc = list.Add("Nature lover");
	tc.Add("David Attenborough");
	tc.Add("Jane Goodall");
	tc.Add("Jacques Cousteau");
	tc.Add("Sir David Attenborough");
	tc.Add("Bear Grylls");}

	{auto& tc = list.Add("Political commentator");
	tc.Add("Anderson Cooper");
	tc.Add("Rachel Maddow");
	tc.Add("Don Lemon");
	tc.Add("Sean Hannity");
	tc.Add("Tucker Carlson");}

	{auto& tc = list.Add("Relationship expert");
	tc.Add("Esther Perel");
	tc.Add("John Gottman");
	tc.Add("Brene Brown");
	tc.Add("Gary Chapman");
	tc.Add("Sue Johnson");}

	{auto& tc = list.Add("Human rights activist");
	tc.Add("Malala Yousafzai");
	tc.Add("Colin Kaepernick");
	tc.Add("Martin Luther King Jr.");
	tc.Add("Greta Thunberg");
	tc.Add("Mahatma Gandhi");}

	{auto& tc = list.Add("Social justice warrior");
	tc.Add("Colin Kaepernick");
	tc.Add("Malala Yousafzai");
	tc.Add("Jesse Williams");
	tc.Add("Emma Gonzalez");
	tc.Add("Megan Rapinoe");}

	{auto& tc = list.Add("Music reviewer");
	tc.Add("Anthony Fantano");
	tc.Add("Rolling Stone magazine");
	tc.Add("Pitchfork");
	tc.Add("NME");
	tc.Add("Complex magazine");}

	{auto& tc = list.Add("Interior design enthusiast");
	tc.Add("Nate Berkus");
	tc.Add("Jonathan Adler");
	tc.Add("Bobby Berk");
	tc.Add("Kelly Wearstler");
	tc.Add("Emily Henderson");}

	{auto& tc = list.Add("Self-help guru");
	tc.Add("Tony Robbins");
	tc.Add("Brené Brown");
	tc.Add("Marie Forleo");
	tc.Add("Mel Robbins");
	tc.Add("Robin Sharma");}

	{auto& tc = list.Add("Life coach");
	tc.Add("Tony Robbins");
	tc.Add("Mel Robbins");
	tc.Add("Marie Forleo");
	tc.Add("Gabby Bernstein");
	tc.Add("Deepak Chopra");}

	{auto& tc = list.Add("Mental health advocate");
	tc.Add("Prince Harry");
	tc.Add("Lady Gaga");
	tc.Add("Glenn Close");
	tc.Add("Demi Lovato");
	tc.Add("Stephen Fry");}

	{auto& tc = list.Add("Promoter/event organizer");
	tc.Add("Ryan Seacrest");
	tc.Add("Guy Oseary");
	tc.Add("Simon Cowell");
	tc.Add("Mark Burnett");
	tc.Add("Live Nation");}

	{auto& tc = list.Add("Financial advisor");
	tc.Add("Warren Buffett");
	tc.Add("Dave Ramsey");
	tc.Add("Suze Orman");
	tc.Add("Robert Kiyosaki");
	tc.Add("Tony Robbins");}

	{auto& tc = list.Add("Food blogger");
	tc.Add("Ina Garten");
	tc.Add("Chrissy Teigen");
	tc.Add("Ree Drummond");
	tc.Add("Joy Wilson (Joy the Baker)");
	tc.Add("Melissa Clark");}

	{auto& tc = list.Add("Sports enthusiast");
	tc.Add("Steph Curry");
	tc.Add("Michael Phelps");
	tc.Add("Serena Williams");
	tc.Add("Usain Bolt");
	tc.Add("Tom Brady");}

	{auto& tc = list.Add("Fashion designer");
	tc.Add("Tom Ford");
	tc.Add("Alexander Wang");
	tc.Add("Marc Jacobs");
	tc.Add("Christian Siriano");
	tc.Add("Michael Kors");}

	{auto& tc = list.Add("Makeup artist");
	tc.Add("Bobbi Brown");
	tc.Add("Pat McGrath");
	tc.Add("Charlotte Tilbury");
	tc.Add("Mario Dedivanovic");
	tc.Add("Lisa Eldridge");}

	{auto& tc = list.Add("Gardening enthusiast");
	tc.Add("Monty Don");
	tc.Add("Chris Beardshaw");
	tc.Add("Joe Lamp'l");
	tc.Add("Diarmuid Gavin");
	tc.Add("Carol Klein");}

	{auto& tc = list.Add("Geek/nerd");
	tc.Add("Neil deGrasse Tyson");
	tc.Add("Mark Zuckerberg");
	tc.Add("Elon Musk");
	tc.Add("Bill Nye");
	tc.Add("Linus Thorvalds");}

	{auto& tc = list.Add("History buff");
	tc.Add("Ken Burns");
	tc.Add("Dan Carlin");
	tc.Add("David McCullough");
	tc.Add("Hampton Sides");
	tc.Add("Simon Sebag Montefiore");}

	{auto& tc = list.Add("Business owner");
	tc.Add("Mark Cuban");
	tc.Add("Tara Reed");
	tc.Add("Lori Cheek");
	tc.Add("Gary Vaynerchuk");
	tc.Add("Renae Christine");}

	{auto& tc = list.Add("Legal expert");
	tc.Add("Dan Abrams");
	tc.Add("Alan Dershowitz");
	tc.Add("Gloria Allred");
	tc.Add("Jose Baez");
	tc.Add("Kim Kardashian-West (as a legal advocate)");}

	{auto& tc = list.Add("Parenting blogger");
	tc.Add("Ashton Kutcher");
	tc.Add("Chip Gaines");
	tc.Add("Kevin Jonas");
	tc.Add("Dwayne \"The Rock\" Johnson");
	tc.Add("Neil Patrick Harris");}

	{auto& tc = list.Add("Senior citizen/retiree");
	tc.Add("Betty White");
	tc.Add("Jane Fonda");
	tc.Add("Morgan Freeman");
	tc.Add("Ian McKellen");
	tc.Add("Angela Lansbury");}

	{auto& tc = list.Add("Marriage counselor");
	tc.Add("John Gottman");
	tc.Add("Esther Perel");
	tc.Add("Brene Brown");
	tc.Add("Gary Chapman");
	tc.Add("Sue Johnson");}

	{auto& tc = list.Add("Wine connoisseur");
	tc.Add("Gary Vaynerchuk");
	tc.Add("Jancis Robinson");
	tc.Add("James Suckling");
	tc.Add("Tim Atkin");
	tc.Add("Karen MacNeil");}

	{auto& tc = list.Add("Youth advocate");
	tc.Add("Malala Yousafzai");
	tc.Add("Greta Thunberg");
	tc.Add("David Hogg");
	tc.Add("Emma Gonzalez");
	tc.Add("Ruth Bader Ginsburg");}

	{auto& tc = list.Add("Success coach");
	tc.Add("Tony Robbins");
	tc.Add("Tim Ferriss");
	tc.Add("Marie Forleo");
	tc.Add("Deepak Chopra");
	tc.Add("Darren Hardy");}

	{auto& tc = list.Add("Career woman/man");
	tc.Add("Sheryl Sandberg");
	tc.Add("Warren Buffett");
	tc.Add("Elon Musk");
	tc.Add("Tim Cook");
	tc.Add("Jack Ma");}

	{auto& tc = list.Add("Fitness coach");
	tc.Add("Jillian Michaels");
	tc.Add("Tony Horton");
	tc.Add("Shaun T");
	tc.Add("Tracy Anderson");
	tc.Add("Kayla Itsines");}

	{auto& tc = list.Add("Political blogger");
	tc.Add("Michelle Obama");
	tc.Add("Joe Scarborough");
	tc.Add("Meghan McCain");
	tc.Add("Ana Navarro");
	tc.Add("Van Jones");}

	{auto& tc = list.Add("Blogger/influencer relations");
	tc.Add("Joe Pulizzi");
	tc.Add("Lee Odden");
	tc.Add("Kristi Hines");
	tc.Add("Larry Kim");
	tc.Add("Neal Schaffer");}

	{auto& tc = list.Add("Adult entertainer");
	tc.Add("Ron Jeremy");
	tc.Add("Sasha Grey");
	tc.Add("Tera Patrick");
	tc.Add("Jenna Jameson");
	tc.Add("Jesse Jane");}

	{auto& tc = list.Add("Adult content creator");
	tc.Add("Belle Delphine");
	tc.Add("OnlyFans");
	tc.Add("AbbyPoblador");
	tc.Add("Sara Jean Underwood");
	tc.Add("Casey Calvert");}

	{auto& tc = list.Add("Adult industry critic");
	tc.Add("Erin Gloria Ryan");
	tc.Add("EJ Dickson");
	tc.Add("Lux Alptraum");
	tc.Add("Amanda Marcotte");
	tc.Add("Dan Savage");}

	{auto& tc = list.Add("Adult content reviewer");
	tc.Add("Sunny Leone");
	tc.Add("Riley Reid");
	tc.Add("Blair Williams");
	tc.Add("April Flores");
	tc.Add("Kayden Kross");}
	
	return list;
}

VectorMap<String,Vector<String>>& GetRolesSafeFemale() {
	thread_local static VectorMap<String,Vector<String>> list;
	if (!list.IsEmpty()) return list;

	{auto& tc = list.Add("Influencer");
	tc.Add("Ellen DeGeneres");
	tc.Add("Taylor Swift");
	tc.Add("Beyoncé");
	tc.Add("Jennifer Aniston");
	tc.Add("Oprah Winfrey");}

	{auto& tc = list.Add("Activist");
	tc.Add("Malala Yousafzai");
	tc.Add("Emma Watson");
	tc.Add("Greta Thunberg");
	tc.Add("Meghan Markle");
	tc.Add("Ellen Page");}

	{auto& tc = list.Add("Expert");
	tc.Add("Brené Brown");
	tc.Add("Dr. Jane Goodall");
	tc.Add("Melinda Gates");
	tc.Add("Michelle Obama");
	tc.Add("Dr. Jill Biden");}

	{auto& tc = list.Add("Comedian");
	tc.Add("Amy Poehler");
	tc.Add("Mindy Kaling");
	tc.Add("Tina Fey");
	tc.Add("Kristen Wiig");
	tc.Add("Ali Wong");}

	{auto& tc = list.Add("Politician");
	tc.Add("Kamala Harris");
	tc.Add("Alexandria Ocasio-Cortez");
	tc.Add("Nancy Pelosi");
	tc.Add("Jacinda Ardern");
	tc.Add("Angela Merkel");}

	{auto& tc = list.Add("Social Media Personality");
	tc.Add("Jenna Marbles");
	tc.Add("Lilly Singh");
	tc.Add("Zoella");
	tc.Add("Liza Koshy");
	tc.Add("Lele Pons");}

	{auto& tc = list.Add("Marketer");
	tc.Add("Sheryl Sandberg");
	tc.Add("Joanne Bradford");
	tc.Add("Carla Hendra");
	tc.Add("Mari Smith");
	tc.Add("Randi Zuckerberg");}

	{auto& tc = list.Add("Journalist");
	tc.Add("Christiane Amanpour");
	tc.Add("Rachel Maddow");
	tc.Add("Gwen Ifill");
	tc.Add("Soledad O'Brien");
	tc.Add("Christiane Amanpour");}

	{auto& tc = list.Add("Writer/Author");
	tc.Add("J.K. Rowling");
	tc.Add("Margaret Atwood");
	tc.Add("Chimamanda Ngozi Adichie");
	tc.Add("Roxane Gay");
	tc.Add("Elizabeth Gilbert");}

	{auto& tc = list.Add("Celebrity/Entertainer");
	tc.Add("Jennifer Lawrence");
	tc.Add("Lucy Hale");
	tc.Add("Mindy Kaling");
	tc.Add("Jennifer Lopez");
	tc.Add("Kerry Washington");}

	{auto& tc = list.Add("Blogger");
	tc.Add("Chiara Ferragni (The Blonde Salad)");
	tc.Add("Aimee Song (Song of Style)");
	tc.Add("Julia Engel (Gal Meets Glam)");
	tc.Add("Camila Coelho");
	tc.Add("Sazan Hendrix");}

	{auto& tc = list.Add("Entrepreneur");
	tc.Add("Sara Blakely (Spanx)");
	tc.Add("Sophia Amoruso (Girlboss)");
	tc.Add("Whitney Wolfe Herd (Bumble)");
	tc.Add("Katrina Lake (Stitch Fix)");
	tc.Add("Jessica Alba (The Honest Company)");}

	{auto& tc = list.Add("Educator");
	tc.Add("Melinda Gates");
	tc.Add("Angela Duckworth");
	tc.Add("Esther Wojcicki");
	tc.Add("Oprah Winfrey");
	tc.Add("Michelle Obama");}

	{auto& tc = list.Add("Student");
	tc.Add("Greta Thunberg");
	tc.Add("Malala Yousafzai");
	tc.Add("Emma González");
	tc.Add("Malala Fund");
	tc.Add("Team Rubicon");}

	{auto& tc = list.Add("Parent");
	tc.Add("Kristen Bell");
	tc.Add("Chrissy Teigen");
	tc.Add("Hilaria Baldwin");
	tc.Add("Gabrielle Union");
	tc.Add("Jessica Alba");}

	{auto& tc = list.Add("Athlete");
	tc.Add("Serena Williams");
	tc.Add("Simone Biles");
	tc.Add("Lindsey Vonn");
	tc.Add("Alex Morgan");
	tc.Add("Megan Rapinoe");}

	{auto& tc = list.Add("Music fan");
	tc.Add("Adele");
	tc.Add("Beyoncé");
	tc.Add("Rihanna");
	tc.Add("Ariana Grande");
	tc.Add("Billie Eilish");}

	{auto& tc = list.Add("Foodie");
	tc.Add("Padma Lakshmi");
	tc.Add("Chrissy Teigen");
	tc.Add("Ina Garten");
	tc.Add("Rachel Ray");
	tc.Add("Chrissy Tiegen");}

	{auto& tc = list.Add("Traveler");
	tc.Add("Rosa Park (Rosa's Musings)");
	tc.Add("Liz Carlson (Young Adventuress)");
	tc.Add("Kristin Luna (Camels & Chocolate)");
	tc.Add("Kiersten Rich (The Blonde Abroad)");
	tc.Add("Julia Dimon (Travel Junkie Julia)");}

	{auto& tc = list.Add("Developer/Programmer");
	tc.Add("Reshma Saujani (Girls Who Code)");
	tc.Add("Laura Kassovic (MbientLab)");
	tc.Add("Stephanie Hurlburt (Binomial)");
	tc.Add("Komal Singh (Square)");
	tc.Add("Tracy Chou (US Digital Service)");}

	{auto& tc = list.Add("Entity/Creative");
	tc.Add("Tavi Gevinson (Rookie Magazine)");
	tc.Add("Jenna Wortham (The New York Times)");
	tc.Add("Roxane Gay (Writer)");
	tc.Add("Mindy Kaling (Actress/Writer/Producer)");
	tc.Add("Olivia Wilde (Actress/Director/Producer)");}

	{auto& tc = list.Add("Scientist/Researcher");
	tc.Add("Jane Goodall");
	tc.Add("Temple Grandin");
	tc.Add("Jennifer Doudna");
	tc.Add("Mayim Bialik (PhD in Neuroscience)");
	tc.Add("Jill Bolte Taylor (Neuroscientist)");}

	{auto& tc = list.Add("Environmentalist");
	tc.Add("Jane Goodall");
	tc.Add("Greta Thunberg");
	tc.Add("Sylvia Earle");
	tc.Add("Isatou Ceesay (Plastic Recycling Advocate)");
	tc.Add("Tessa Tennant (Sustainable Investment Pioneer)");}

	{auto& tc = list.Add("Animal Lover/Activist");
	tc.Add("Jane Goodall");
	tc.Add("Temple Grandin");
	tc.Add("Ellie Goulding (UN Environment Global Goodwill Ambassador)");
	tc.Add("Rachael Ray (Animal Welfare Advocate)");
	tc.Add("Maisie Williams (Animal Rights Activist)");}

	{auto& tc = list.Add("Fashionista");
	tc.Add("Chiara Ferragni");
	tc.Add("Blake Lively");
	tc.Add("Anna Wintour (Editor-in-Chief of Vogue)");
	tc.Add("Rachel Zoe (Fashion Designer/Stylist/TV Personality)");
	tc.Add("Diane von Furstenberg (Fashion Designer/Philanthropist)");}

	{auto& tc = list.Add("Homemaker");
	tc.Add("Joanna Gaines");
	tc.Add("Martha Stewart");
	tc.Add("Ina Garten");
	tc.Add("Ree Drummond (The Pioneer Woman)");
	tc.Add("Nigella Lawson");}

	{auto& tc = list.Add("Philanthropist");
	tc.Add("Melinda Gates");
	tc.Add("Oprah Winfrey");
	tc.Add("Priscilla Chan (Chan Zuckerberg Initiative)");
	tc.Add("Laurene Powell Jobs (Emerson Collective)");
	tc.Add("Sheryl Sandberg (Lean In Foundation)");}

	{auto& tc = list.Add("Socialite");
	tc.Add("Amal Clooney");
	tc.Add("Meghan Markle");
	tc.Add("Paris Hilton");
	tc.Add("Huda Kattan");
	tc.Add("Anouska Paris");}

	{auto& tc = list.Add("Food/Drink Critic");
	tc.Add("Padma Lakshmi");
	tc.Add("Ina Garten");
	tc.Add("Giada De Laurentiis");
	tc.Add("Chrissy Teigen");
	tc.Add("Rachael Ray");}

	{auto& tc = list.Add("Gamer");
	tc.Add("Rachel Quirico");
	tc.Add("Ashley Jenkins");
	tc.Add("Mari Takahashi");
	tc.Add("Andrea Rene");
	tc.Add("Trisha Hershberger");}

	{auto& tc = list.Add("Fitness Enthusiast");
	tc.Add("Kayla Itsines");
	tc.Add("Karena Dawn and Katrina Scott (Tone It Up)");
	tc.Add("Massy Arias (Manko Fit)");
	tc.Add("Jen Widerstrom");
	tc.Add("Linn Jacobsson (Linnea Enmark)");}

	{auto& tc = list.Add("Health/Wellness Guru");
	tc.Add("Dr. Oz");
	tc.Add("Dr. Deepa Verma");
	tc.Add("Dr. Cathy Kapica");
	tc.Add("Dr. Lisa A. Price");
	tc.Add("Dr. Shiza Shahid");}

	{auto& tc = list.Add("Spiritual Leader");
	tc.Add("Oprah Winfrey");
	tc.Add("Marianne Williamson");
	tc.Add("Deepak Chopra");
	tc.Add("Gabrielle Bernstein");
	tc.Add("Eckhart Tolle");}

	{auto& tc = list.Add("Parenting Advice");
	tc.Add("Joanna Gaines");
	tc.Add("Jessica Alba");
	tc.Add("Mayim Bialik");
	tc.Add("Ellen DeGeneres");
	tc.Add("Busy Philipps");}

	{auto& tc = list.Add("Career Coach/Advisor");
	tc.Add("Oprah Winfrey");
	tc.Add("Melinda Gates");
	tc.Add("Sheryl Sandberg");
	tc.Add("Marie Forleo");
	tc.Add("Brené Brown");}

	{auto& tc = list.Add("Travel Blogger");
	tc.Add("Liz Carlson (Young Adventuress)");
	tc.Add("Kristin Addis (Be My Travel Muse)");
	tc.Add("Julia Engel (Gal Meets Glam)");
	tc.Add("Kate McCulley (Adventure Kate)");
	tc.Add("Polkadot Passport (Nicole Warne)");}

	{auto& tc = list.Add("Book Lover/Reader");
	tc.Add("Emma Watson");
	tc.Add("Reese Witherspoon");
	tc.Add("Oprah Winfrey");
	tc.Add("Jenna Bush Hager");
	tc.Add("Jessica Alba");}

	{auto& tc = list.Add("DIY Enthusiast");
	tc.Add("Martha Stewart");
	tc.Add("Homey Oh My (Amy Kim)");
	tc.Add("Jennifer Perkins");
	tc.Add("Studio DIY (Kelly Mindell)");
	tc.Add("A Beautiful Mess (Jess and Emma)");}

	{auto& tc = list.Add("Pet Lover/Owner");
	tc.Add("Ellen DeGeneres");
	tc.Add("Amanda Seyfried");
	tc.Add("Ricky Gervais");
	tc.Add("Ian Somerhalder");
	tc.Add("Marnie The Dog");}

	{auto& tc = list.Add("Movie/TV Critic");
	tc.Add("Rotten Tomatoes");
	tc.Add("MovieBob (Bob Chipman)");
	tc.Add("Renegade Cut (Leon Thomas)");
	tc.Add("Nostalgia Critic (Doug Walker)");
	tc.Add("Anya Volz");}

	{auto& tc = list.Add("Beauty/Fashion Blogger");
	tc.Add("Huda Kattan");
	tc.Add("Michelle Phan");
	tc.Add("Zoella");
	tc.Add("Camila Coelho");
	tc.Add("Jackie Aina");}

	{auto& tc = list.Add("Tech Geek");
	tc.Add("Kara Swisher");
	tc.Add("Anil Dash");
	tc.Add("Marques Brownlee (MKBHD)");
	tc.Add("Justine Ezarik (iJustine)");
	tc.Add("Emily Lakdawalla (The Planetary Society)");}

	{auto& tc = list.Add("Nature Lover");
	tc.Add("Jane Goodall");
	tc.Add("Anna McNuff (British Adventurer)");
	tc.Add("Alexia Zuberer (The Wayward Walrus)");
	tc.Add("Lauren Bath (Digital Marketing Consultant)");
	tc.Add("Anna McNuff (Champions Mentality)");}

	{auto& tc = list.Add("Political Commentator");
	tc.Add("Joy Reid");
	tc.Add("Ana Navarro");
	tc.Add("Rachel Maddow");
	tc.Add("Amy Goodman");
	tc.Add("Maggie Haberman");}

	{auto& tc = list.Add("Relationship Expert");
	tc.Add("Esther Perel");
	tc.Add("Dr. Jenn Mann");
	tc.Add("Dr. Laura Berman");
	tc.Add("Marie Forleo");
	tc.Add("Sarah Jones");}

	{auto& tc = list.Add("Human Rights Activist");
	tc.Add("Malala Yousafzai");
	tc.Add("Emma Gonzalez");
	tc.Add("Tarana Burke (founder of #MeToo movement)");
	tc.Add("Amnesty International");
	tc.Add("Laverne Cox");}

	{auto& tc = list.Add("Social Justice Warrior");
	tc.Add("Patrisse Cullors (Black Lives Matter)");
	tc.Add("DeRay Mckesson (Civil Rights Activist)");
	tc.Add("Linda Sarsour (Women's Rights Activist)");
	tc.Add("Cecile Richards (Planned Parenthood)");
	tc.Add("Opal Tometi (Black Lives Matter)");}

	{auto& tc = list.Add("Music Reviewer");
	tc.Add("Jon Pareles (The New York Times)");
	tc.Add("Jeremy Larson (Pitchfork)");
	tc.Add("Jenny Eliscu (Rolling Stone)");
	tc.Add("Dave Bry (The Guardian)");
	tc.Add("Lindsay Zoladz (Vulture)");}

	{auto& tc = list.Add("Interior Design Enthusiast");
	tc.Add("Joanna Gaines");
	tc.Add("Amber Lewis");
	tc.Add("Shea McGee");
	tc.Add("Kate Marker");
	tc.Add("Erin Hiemstra (Apartment 34)");}

	{auto& tc = list.Add("Self-Help Guru");
	tc.Add("Brené Brown");
	tc.Add("Marie Forleo");
	tc.Add("Rachel Hollis");
	tc.Add("Gabby Bernstein");
	tc.Add("Mel Robbins");}

	{auto& tc = list.Add("Life Coach");
	tc.Add("Marie Forleo");
	tc.Add("Tony Robbins");
	tc.Add("Gabby Bernstein");
	tc.Add("Mel Robbins");
	tc.Add("Brendon Burchard");}

	{auto& tc = list.Add("Mental Health Advocate");
	tc.Add("Glennon Doyle");
	tc.Add("Simone Biles");
	tc.Add("Jen Gotch (founder of Ban.do)");
	tc.Add("Michelle Williams");
	tc.Add("Demi Lovato");}

	{auto& tc = list.Add("Promoter/Event Organizer");
	tc.Add("Oprah Winfrey");
	tc.Add("Michelle Obama");
	tc.Add("Marie Forleo");
	tc.Add("Melinda Gates");
	tc.Add("Arianna Huffington");}

	{auto& tc = list.Add("Financial Advisor");
	tc.Add("Suze Orman");
	tc.Add("Elizabeth Warren");
	tc.Add("Mellody Hobson");
	tc.Add("Rachel Cruze");
	tc.Add("Farnoosh Torabi");}

	{auto& tc = list.Add("Food Blogger");
	tc.Add("Joy Bauer");
	tc.Add("Alison Roman");
	tc.Add("Tieghan Gerard (Half Baked Harvest)");
	tc.Add("Angela Davis (The Kitchenista Diaries)");
	tc.Add("Joy McCarthy (Joyous Health)");}

	{auto& tc = list.Add("Sports Enthusiast");
	tc.Add("Katie Sowers");
	tc.Add("Julie Ertz");
	tc.Add("Lindsey Vonn");
	tc.Add("Sue Bird");
	tc.Add("Alex Morgan");}

	{auto& tc = list.Add("Fashion Designer");
	tc.Add("Tory Burch");
	tc.Add("Stella McCartney");
	tc.Add("Diane Von Furstenberg");
	tc.Add("Donna Karan");
	tc.Add("Carolina Herrera");}

	{auto& tc = list.Add("Makeup Artist");
	tc.Add("Bobbi Brown");
	tc.Add("Pat McGrath");
	tc.Add("Lisa Eldridge");
	tc.Add("Charlotte Tilbury");
	tc.Add("Jaclyn Hill");}

	{auto& tc = list.Add("Gardening Enthusiast");
	tc.Add("Monty Don");
	tc.Add("Martha Stewart");
	tc.Add("Melinda Myers");
	tc.Add("Nikki Jardin");
	tc.Add("Maureen Gilmer");}

	{auto& tc = list.Add("Geek/Nerd");
	tc.Add("Felicia Day");
	tc.Add("Mindy Kaling");
	tc.Add("Aisha Tyler");
	tc.Add("Ashley Eckstein");
	tc.Add("Gwendoline Christie");}

	{auto& tc = list.Add("History Buff");
	tc.Add("Melinda Gates");
	tc.Add("Emma Gonzalez");
	tc.Add("Brené Brown");
	tc.Add("Michelle Obama");
	tc.Add("Jane Goodall");}

	{auto& tc = list.Add("Business Owner");
	tc.Add("Sara Blakely (Spanx)");
	tc.Add("Sophia Amoruso (Girlboss)");
	tc.Add("Katrina Lake (Stitch Fix)");
	tc.Add("Jenn Hyman (Rent The Runway)");
	tc.Add("Jessica Alba (The Honest Company)");}

	{auto& tc = list.Add("Legal Expert");
	tc.Add("Ruth Bader Ginsburg");
	tc.Add("Sonia Sotomayor");
	tc.Add("Jeanine Pirro");
	tc.Add("Ari Melber");
	tc.Add("Laura Coates");}

	{auto& tc = list.Add("Parenting Blogger");
	tc.Add("Anna White (Maestro Classics)");
	tc.Add("Ellen Seidman (Love That Max)");
	tc.Add("Gabrielle Blair (Design Mom)");
	tc.Add("Kelly Wickham Hurst (Mocha Momma)");
	tc.Add("Sarah Bessey (An Inch of Gray)");}

	{auto& tc = list.Add("Senior Citizen/Retiree");
	tc.Add("Rita Moreno");
	tc.Add("Judi Dench");
	tc.Add("Maggie Smith");
	tc.Add("Betty White");
	tc.Add("Jane Fonda");}

	{auto& tc = list.Add("Marriage Counselor");
	tc.Add("Esther Perel");
	tc.Add("Dr. Sue Johnson");
	tc.Add("Rachel Sussman");
	tc.Add("Les Parrot");
	tc.Add("Gary Chapman");}

	{auto& tc = list.Add("Wine Connoisseur");
	tc.Add("Jancis Robinson");
	tc.Add("Jamie Goode");
	tc.Add("Alice Feiring");
	tc.Add("Elizabeth Schneider");
	tc.Add("Emma Rice");}

	{auto& tc = list.Add("Youth advocate");
	tc.Add("Malala Yousafzai");
	tc.Add("Greta Thunberg");
	tc.Add("Mari Copeny");
	tc.Add("Jazz Jennings");
	tc.Add("Yara Shahidi");}

	{auto& tc = list.Add("Success coach");
	tc.Add("Mel Robbins");
	tc.Add("Marie Forleo");
	tc.Add("Tony Robbins");
	tc.Add("Gabrielle Bernstein");
	tc.Add("Brené Brown");}

	{auto& tc = list.Add("Career woman");
	tc.Add("Indra Nooyi");
	tc.Add("Sheryl Sandberg");
	tc.Add("Arianna Huffington");
	tc.Add("Mary Barra");
	tc.Add("Oprah Winfrey");}

	{auto& tc = list.Add("Fitness coach");
	tc.Add("Jillian Michaels");
	tc.Add("Kayla Itsines");
	tc.Add("Tracy Anderson");
	tc.Add("Simone De La Rue");
	tc.Add("Koya Webb");}

	{auto& tc = list.Add("Political blogger");
	tc.Add("Rachel Maddow");
	tc.Add("Ana Navarro");
	tc.Add("Symone Sanders");
	tc.Add("Sarah Cooper");
	tc.Add("Melissa Harris-Perry");}

	{auto& tc = list.Add("Blogger/influencer relations");
	tc.Add("Aimee Song");
	tc.Add("Chiara Ferragni");
	tc.Add("Amber Fillerup");
	tc.Add("Negin Mirsalehi");
	tc.Add("Danielle Bernstein");}

	{auto& tc = list.Add("Adult entertainer");
	tc.Add("Jenna Jameson");
	tc.Add("Stormy Daniels");
	tc.Add("Janice Griffith");
	tc.Add("Asa Akira");
	tc.Add("Briana Banks");}

	{auto& tc = list.Add("Adult content creator");
	tc.Add("Asa Akira");
	tc.Add("Angela White");
	tc.Add("Lena Paul");
	tc.Add("Kendra Sunderland");
	tc.Add("Casey Calvert");}

	{auto& tc = list.Add("Adult industry critic");
	tc.Add("Nina Hartley");
	tc.Add("Stormy Daniels");
	tc.Add("Kendra Holliday");
	tc.Add("Jaclyn Friedman");
	tc.Add("Jenna Jameson");}

	{auto& tc = list.Add("Adult content reviewer");
	tc.Add("Aria Taylor");
	tc.Add("Lily LaBeau");
	tc.Add("Riley Reid");
	tc.Add("Asa Akira");
	tc.Add("Stoya");}
	
	return list;
}

VectorMap<String,Vector<String>>& GetRolesUnsafeMale() {
	thread_local static VectorMap<String,Vector<String>> list;
	if (!list.IsEmpty()) return list;
	{auto& tc = list.Add("Influencer");
	tc.Add("Logan Paul");
	tc.Add("Jake Paul");
	tc.Add("Jeffree Star");
	tc.Add("PewDiePie");
	tc.Add("Shane Dawson");}

	{auto& tc = list.Add("Activist");
	tc.Add("Alex Jones");
	tc.Add("Milo Yiannopoulos");
	tc.Add("Tommy Robinson");
	tc.Add("Richard Spencer");
	tc.Add("Gavin McInnes");}

	{auto& tc = list.Add("Expert");
	tc.Add("Jordan Peterson");
	tc.Add("Sam Harris");
	tc.Add("Mike Cernovich");
	tc.Add("Ben Shapiro");
	tc.Add("Stefan Molyneux");}

	{auto& tc = list.Add("Comedian");
	tc.Add("Louis C.K.");
	tc.Add("Bill Maher");
	tc.Add("Ricky Gervais");
	tc.Add("Jim Jefferies");
	tc.Add("Russell Brand");}

	{auto& tc = list.Add("Politician");
	tc.Add("Donald Trump");
	tc.Add("Nigel Farage");
	tc.Add("Boris Johnson");
	tc.Add("Matteo Salvini");
	tc.Add("Jair Bolsonaro ");}

	{auto& tc = list.Add("Social media personality");
	tc.Add("Logan Paul");
	tc.Add("Jake Paul");
	tc.Add("James Charles");
	tc.Add("Tana Mongeau");
	tc.Add("Lele Pons");}

	{auto& tc = list.Add("Marketer");
	tc.Add("Tai Lopez");
	tc.Add("Tim Sykes");
	tc.Add("Grant Cardone");
	tc.Add("Dan Bilzerian");
	tc.Add("Anthony Morrison");}

	{auto& tc = list.Add("Journalist");
	tc.Add("Tomi Lahren");
	tc.Add("Charlie Kirk");
	tc.Add("Milo Yiannopoulos");
	tc.Add("Alex Jones");
	tc.Add("Paul Joseph Watson");}

	{auto& tc = list.Add("Writer/author");
	tc.Add("Jordan Peterson");
	tc.Add("Ben Shapiro");
	tc.Add("Milo Yiannopoulos");
	tc.Add("David Horowitz");
	tc.Add("Alex Jones");}

	{auto& tc = list.Add("Celebrity/entertainer");
	tc.Add("Kanye West");
	tc.Add("Chris Brown");
	tc.Add("Johnny Depp");
	tc.Add("Mel Gibson");
	tc.Add("Alec Baldwin");}

	{auto& tc = list.Add("Blogger");
	tc.Add("Milo Yiannopoulos");
	tc.Add("Mike Cernovich");
	tc.Add("Dave Rubin");
	tc.Add("Paul Joseph Watson");
	tc.Add("James Allsup");}

	{auto& tc = list.Add("Entrepreneur");
	tc.Add("Gary Vaynerchuk");
	tc.Add("Tai Lopez");
	tc.Add("Tim Ferriss");
	tc.Add("Grant Cardone");
	tc.Add("Daymond John");}

	{auto& tc = list.Add("Educator");
	tc.Add("Jordan Peterson");
	tc.Add("Ben Shapiro");
	tc.Add("Dave Rubin");
	tc.Add("Tomi Lahren");
	tc.Add("Milo Yiannopoulos");}

	{auto& tc = list.Add("Student");
	tc.Add("Milo Yiannopoulos");
	tc.Add("Richard Spencer");
	tc.Add("Jack Posobiec");
	tc.Add("Laura Loomer");
	tc.Add("Ben Shapiro");}

	{auto& tc = list.Add("Parent");
	tc.Add("Jordan Peterson");
	tc.Add("Ben Shapiro");
	tc.Add("Milo Yiannopoulos");
	tc.Add("Mike Cernovich");
	tc.Add("Alex Jones");}

	{auto& tc = list.Add("Athlete");
	tc.Add("Colin Kaepernick");
	tc.Add("Kyrie Irving");
	tc.Add("Dennis Rodman");
	tc.Add("Marshawn Lynch");
	tc.Add("Greg Hardy");}

	{auto& tc = list.Add("Music fan");
	tc.Add("Kanye West");
	tc.Add("Chris Brown");
	tc.Add("Tyga");
	tc.Add("Migos");
	tc.Add("Kodak Black");}

	{auto& tc = list.Add("Foodie");
	tc.Add("Gordon Ramsay");
	tc.Add("Guy Fieri");
	tc.Add("Anthony Bourdain");
	tc.Add("Bobby Flay");
	tc.Add("Wolfgang Puck");}

	{auto& tc = list.Add("Traveler");
	tc.Add("Anthony Bourdain");
	tc.Add("Rick Steves");
	tc.Add("Bear Grylls");
	tc.Add("Johnny Depp");
	tc.Add("Jack Osbourne");}

	{auto& tc = list.Add("Developer/programmer");
	tc.Add("Mark Zuckerberg");
	tc.Add("Elon Musk");
	tc.Add("Jack Dorsey");
	tc.Add("Bill Gates");
	tc.Add("Jeff Bezos");}

	{auto& tc = list.Add("Entity/creative");
	tc.Add("Kanye West");
	tc.Add("Donald Glover");
	tc.Add("Banksy");
	tc.Add("Tyler The Creator");
	tc.Add("Amanda Palmer");}

	{auto& tc = list.Add("Scientist/researcher");
	tc.Add("Richard Dawkins");
	tc.Add("Sam Harris");
	tc.Add("Neil deGrasse Tyson");
	tc.Add("Stephen Hawking");
	tc.Add("Bill Nye");}

	{auto& tc = list.Add("Environmentalist");
	tc.Add("Donald Trump");
	tc.Add("Rush Limbaugh");
	tc.Add("Alex Jones");
	tc.Add("Mike Pence");
	tc.Add("Ted Cruz");}

	{auto& tc = list.Add("Animal lover/activist");
	tc.Add("Ricky Gervais");
	tc.Add("Steve Irwin");
	tc.Add("PETA");
	tc.Add("Robert Irwin");
	tc.Add("Jim Carrey");}

	{auto& tc = list.Add("Fashionista");
	tc.Add("Kanye West");
	tc.Add("Jared Leto");
	tc.Add("Pharrell Williams");
	tc.Add("Billy Porter");
	tc.Add("Harry Styles");}

	{auto& tc = list.Add("Homemaker");
	tc.Add("Jordan Peterson");
	tc.Add("Dave Ramsey");
	tc.Add("Ron Paul");
	tc.Add("Alex Jones");
	tc.Add("Ben Shapiro");}

	{auto& tc = list.Add("Philanthropist");
	tc.Add("George Soros");
	tc.Add("Bill Gates");
	tc.Add("Warren Buffett");
	tc.Add("Mark Zuckerberg");
	tc.Add("Carlos Slim");}

	{auto& tc = list.Add("Socialite");
	tc.Add("Paris Hilton");
	tc.Add("Kim Kardashian-West");
	tc.Add("Khloe Kardashian");
	tc.Add("Kylie Jenner");
	tc.Add("Kendall Jenner");}

	{auto& tc = list.Add("Food/drink critic");
	tc.Add("Anthony Bourdain");
	tc.Add("Gordon Ramsay");
	tc.Add("Guy Fieri");
	tc.Add("Bobby Flay");
	tc.Add("Andrew Zimmern");}

	{auto& tc = list.Add("Gamer");
	tc.Add("PewDiePie");
	tc.Add("Ninja");
	tc.Add("Markiplier");
	tc.Add("Jacksepticeye");
	tc.Add("FaZe Rug");}

	{auto& tc = list.Add("Fitness enthusiast");
	tc.Add("Greg Doucette");
	tc.Add("Bradley Martyn");
	tc.Add("Christian Guzman");
	tc.Add("CT Fletcher");
	tc.Add("Joe Rogan");}

	{auto& tc = list.Add("Health/wellness guru");
	tc.Add("Dr. Oz");
	tc.Add("Deepak Chopra");
	tc.Add("Gwyneth Paltrow");
	tc.Add("Jillian Michaels");
	tc.Add("Kourtney Kardashian");}

	{auto& tc = list.Add("Spiritual leader");
	tc.Add("Joel Osteen");
	tc.Add("Pat Robertson");
	tc.Add("Jim Bakker");
	tc.Add("Benny Hinn");
	tc.Add("Deepak Chop");}

	{auto& tc = list.Add("Parenting advice");
	tc.Add("Jordan Peterson");
	tc.Add("Ben Shapiro");
	tc.Add("Milo Yiannopoulos");
	tc.Add("Mike Cernovich");
	tc.Add("Alex Jones");}

	{auto& tc = list.Add("Career coach/advisor");
	tc.Add("Gary Vaynerchuk");
	tc.Add("Dan Lok");
	tc.Add("Tony Robbins");
	tc.Add("Grant Cardone");
	tc.Add("Tim Ferriss");}

	{auto& tc = list.Add("Travel blogger");
	tc.Add("Mike Posner");
	tc.Add("Sam Kolder");
	tc.Add("Daniel Kordan");
	tc.Add("Mark Wiens");
	tc.Add("Yellow Brick Cinema");}

	{auto& tc = list.Add("Book lover/reader");
	tc.Add("Jordan Peterson");
	tc.Add("Ben Shapiro");
	tc.Add("Dave Rubin");
	tc.Add("Milo Yiannopoulos");
	tc.Add("Stephen King");}

	{auto& tc = list.Add("DIY enthusiast");
	tc.Add("Adam Savage");
	tc.Add("Bob Vila");
	tc.Add("Ty Pennington");
	tc.Add("Mike Holmes");
	tc.Add("Tim Taylor (from \"Home Improvement\")");}

	{auto& tc = list.Add("Pet lover/owner");
	tc.Add("Joe Exotic");
	tc.Add("Tia Torres");
	tc.Add("Caesar Milan");
	tc.Add("Jackson Galaxy");
	tc.Add("Steve Irwin");}

	{auto& tc = list.Add("Movie/TV critic");
	tc.Add("Charlie Sheen");
	tc.Add("Alec Baldwin");
	tc.Add("Kevin Spacey");
	tc.Add("James Franco");
	tc.Add("Johnny Depp");}

	{auto& tc = list.Add("Beauty/fashion blogger");
	tc.Add("Jeffree Star");
	tc.Add("James Charles");
	tc.Add("Tati Westbrook");
	tc.Add("Huda Kattan");
	tc.Add("Manny MUA");}

	{auto& tc = list.Add("Tech geek");
	tc.Add("Mark Zuckerberg");
	tc.Add("Elon Musk");
	tc.Add("Bill Gates");
	tc.Add("Jeff Bezos");
	tc.Add("Steve Jobs");}

	{auto& tc = list.Add("Nature lover");
	tc.Add("Bear Grylls");
	tc.Add("Steve Irwin");
	tc.Add("David Attenborough");
	tc.Add("David Bellamy");
	tc.Add("Jeff Corwin");}

	{auto& tc = list.Add("Political commentator");
	tc.Add("Ben Shapiro");
	tc.Add("Milo Yiannopoulos");
	tc.Add("Steven Crowder");
	tc.Add("Glenn Beck");
	tc.Add("Rush Limbaugh");}

	{auto& tc = list.Add("Relationship expert");
	tc.Add("Jordan Peterson");
	tc.Add("Ben Shapiro");
	tc.Add("Milo Yiannopoulos");
	tc.Add("Mike Cernovich");
	tc.Add("Alex Jones");}

	{auto& tc = list.Add("Human rights activist");
	tc.Add("George Soros");
	tc.Add("Noam Chomsky");
	tc.Add("Salman Rushdie");
	tc.Add("Chelsea Manning");
	tc.Add("Malala Yousafzai");}

	{auto& tc = list.Add("Social justice warrior");
	tc.Add("Shaun King");
	tc.Add("Linda Sarsour");
	tc.Add("Keith Ellison");
	tc.Add("Al Sharpton");
	tc.Add("Maxine Waters");}

	{auto& tc = list.Add("Music reviewer");
	tc.Add("Anthony Fantano");
	tc.Add("Todd in the Shadows");
	tc.Add("Needle Drop");
	tc.Add("TheReportOfTheWeek");
	tc.Add("theneedledrop");}

	{auto& tc = list.Add("Interior design enthusiast");
	tc.Add("Bobby Berk");
	tc.Add("Nate Berkus");
	tc.Add("Chip and Joanna Gaines");
	tc.Add("Ryan Korban");
	tc.Add("Jeremiah Brent");}

	{auto& tc = list.Add("Self-help guru");
	tc.Add("Tony Robbins");
	tc.Add("Deepak Chopra");
	tc.Add("Joel Osteen");
	tc.Add("Eckhart Tolle");
	tc.Add("Louise Hay");}

	{auto& tc = list.Add("Life coach");
	tc.Add("Jordan Peterson");
	tc.Add("Oprah Winfrey");
	tc.Add("Marie Forleo");
	tc.Add("Mel Robbins");
	tc.Add("Tony Robbins");}

	{auto& tc = list.Add("Mental health advocate");
	tc.Add("Pete Davidson");
	tc.Add("Demi Lovato");
	tc.Add("Lady Gaga");
	tc.Add("Michael Phelps");
	tc.Add("Dwayne \"The Rock\" Johnson");}

	{auto& tc = list.Add("Promoter/event organizer");
	tc.Add("Billy McFarland (Fyre Festival)");
	tc.Add("Ja Rule (Fyre Festival)");
	tc.Add("Jordan Belfort (The Wolf of Wall Street)");
	tc.Add("Harvey Weinstein");
	tc.Add("Donald Trump (Miss Universe pageant)");}

	{auto& tc = list.Add("Financial advisor");
	tc.Add("Grant Cardone");
	tc.Add("Dave Ramsey");
	tc.Add("Robert Kiyosaki");
	tc.Add("Suze Orman");
	tc.Add("Jim Cramer");}

	{auto& tc = list.Add("Food blogger");
	tc.Add("Adam Richman");
	tc.Add("Andrew Zimmern");
	tc.Add("Anthony Bourdain");
	tc.Add("Samin Nosrat");
	tc.Add("Jamie Oliver");}

	{auto& tc = list.Add("Sports enthusiast");
	tc.Add("Stephen A. Smith");
	tc.Add("Skip Bayless");
	tc.Add("Colin Cowherd");
	tc.Add("Clay Travis");
	tc.Add("Shannon Sharpe");}

	{auto& tc = list.Add("Fashion designer");
	tc.Add("Marc Jacobs");
	tc.Add("Michael Kors");
	tc.Add("Karl Lagerfeld");
	tc.Add("Tom Ford");
	tc.Add("Marc Ecko");}

	{auto& tc = list.Add("Makeup artist");
	tc.Add("Jeffree Star");
	tc.Add("James Charles");
	tc.Add("Manny MUA");
	tc.Add("NikkieTutorials");
	tc.Add("Tati Westbrook");}

	{auto& tc = list.Add("Gardening enthusiast");
	tc.Add("Monty Don");
	tc.Add("Alan Titchmarsh");
	tc.Add("Chris Packham");
	tc.Add("Joe Swift");
	tc.Add("Carol Klein");}

	{auto& tc = list.Add("Geek/nerd");
	tc.Add("Elon Musk");
	tc.Add("Neil DeGrasse Tyson");
	tc.Add("Bill Gates");
	tc.Add("Mark Zuckerberg");
	tc.Add("Richard Dawkins");}

	{auto& tc = list.Add("History buff");
	tc.Add("Dan Carlin");
	tc.Add("Ken Burns");
	tc.Add("David McCullough");
	tc.Add("Ron Chernow");
	tc.Add("Shelby Foote");}

	{auto& tc = list.Add("Business owner");
	tc.Add("Mark Cuban");
	tc.Add("Richard Branson");
	tc.Add("Elon Musk");
	tc.Add("Jeff Bezos");
	tc.Add("Bill Gates");}

	{auto& tc = list.Add("Legal expert");
	tc.Add("Alan Dershowitz");
	tc.Add("Joseph diGenova");
	tc.Add("Rudy Giuliani");
	tc.Add("Ken Starr");
	tc.Add("Jay Sekulow");}

	{auto& tc = list.Add("Parenting blogger");
	tc.Add("Jordan Peterson");
	tc.Add("Ben Shapiro");
	tc.Add("Milo Yiannopoulos");
	tc.Add("Mike Cernovich");
	tc.Add("Alex Jones");}

	{auto& tc = list.Add("Senior citizen/retiree");
	tc.Add("George Soros");
	tc.Add("Warren Buffett");
	tc.Add("Richard Branson");
	tc.Add("Larry King");
	tc.Add("Rupert Murdoch");}

	{auto& tc = list.Add("Marriage counselor");
	tc.Add("Dr. Phil McGraw");
	tc.Add("Dr. Laura Schlessinger");
	tc.Add("Dr. Ruth Westheimer");
	tc.Add("Dr. John Gottman");
	tc.Add("Steve Harvey");}

	{auto& tc = list.Add("Wine connoisseur");
	tc.Add("Gary Vaynerchuk");
	tc.Add("Anthony Bourdain");
	tc.Add("Tim Ferriss");
	tc.Add("Master Sommelier");
	tc.Add("Jancis Robinson");}

	{auto& tc = list.Add("Youth advocate");
	tc.Add("David Hogg");
	tc.Add("Greta Thunberg");
	tc.Add("Parkland students");
	tc.Add("Malala Yousafzai");
	tc.Add("Emma Gonzalez");}

	{auto& tc = list.Add("Success coach");
	tc.Add("Dan Lok");
	tc.Add("Tony Robbins");
	tc.Add("Gary Vaynerchuk");
	tc.Add("Grant Cardone");
	tc.Add("Brendon Burchard");}

	{auto& tc = list.Add("Career woman/man");
	tc.Add("Ivanka Trump");
	tc.Add("Sheryl Sandberg");
	tc.Add("Rachel Hollis");
	tc.Add("Melinda Gates");
	tc.Add("Indra Nooyi");}

	{auto& tc = list.Add("Fitness coach");
	tc.Add("Greg Doucette");
	tc.Add("Bradley Martyn");
	tc.Add("Amanda Bucci");
	tc.Add("CT Fletcher");
	tc.Add("Chris Jones");}

	{auto& tc = list.Add("Political blogger");
	tc.Add("Milo Yiannopoulos");
	tc.Add("Tucker Carlson");
	tc.Add("Piers Morgan");
	tc.Add("Ben Shapiro");
	tc.Add("Steve Bannon");}

	{auto& tc = list.Add("Blogger/influencer relations");
	tc.Add("Jeffree Star");
	tc.Add("James Charles");
	tc.Add("Tana Mongeau");
	tc.Add("Logan Paul");
	tc.Add("Jake Paul");}

	{auto& tc = list.Add("Adult entertainer");
	tc.Add("Stormy Daniels");
	tc.Add("Ron Jeremy");
	tc.Add("Jenna Jameson");
	tc.Add("Lisa Ann");
	tc.Add("Sasha Grey");}

	{auto& tc = list.Add("Adult content creator");
	tc.Add("James Deen");
	tc.Add("Stoya");
	tc.Add("Asa Akira");
	tc.Add("Riley Reid");
	tc.Add("Johnny Sins");}

	{auto& tc = list.Add("Adult industry critic");
	tc.Add("Gail Dines");
	tc.Add("Shelley Lubben");
	tc.Add("Pamela Stenzel");
	tc.Add("Donny Pauling");
	tc.Add("Nicholas Kristof");}

	{auto& tc = list.Add("Adult content reviewer");
	tc.Add("Keemstar");
	tc.Add("iDubbbz");
	tc.Add("h3h3 Productions");
	tc.Add("Ethan Klein");
	tc.Add("Michael McCrudden");}

	return list;
}

VectorMap<String,Vector<String>>& GetRolesUnsafeFemale() {
	thread_local static VectorMap<String,Vector<String>> list;
	if (!list.IsEmpty()) return list;

	{auto& tc = list.Add("Influencer");
	tc.Add("Kylie Jenner");
	tc.Add("Kim Kardashian-West");
	tc.Add("Amanda Bynes");
	tc.Add("Azealia Banks");
	tc.Add("Lindsay Lohan");}

	{auto& tc = list.Add("Activist");
	tc.Add("Azealia Banks");
	tc.Add("Roseanne Barr");
	tc.Add("Chelsea Handler");
	tc.Add("Kathy Griffin");
	tc.Add("Wendy Williams");}

	{auto& tc = list.Add("Expert");
	tc.Add("Ann Coulter");
	tc.Add("Chelsea Handler");
	tc.Add("Wendy Williams");
	tc.Add("Azealia Banks");
	tc.Add("Lindsay Lohan");}

	{auto& tc = list.Add("Comedian");
	tc.Add("Kathy Griffin");
	tc.Add("Chelsea Handler");
	tc.Add("Roseanne Barr");
	tc.Add("Wendy Williams");
	tc.Add("Azealia Banks");}

	{auto& tc = list.Add("Politician");
	tc.Add("Ann Coulter");
	tc.Add("Roseanne Barr");
	tc.Add("Amanda Bynes");
	tc.Add("Kim Kardashian-West");
	tc.Add("Azealia Banks");}

	{auto& tc = list.Add("Social media personality");
	tc.Add("Kylie Jenner");
	tc.Add("Kim Kardashian-West");
	tc.Add("Amanda Bynes");
	tc.Add("Lindsay Lohan");
	tc.Add("Wendy Williams");}

	{auto& tc = list.Add("Marketer");
	tc.Add("Kim Kardashian-West");
	tc.Add("Azealia Banks");
	tc.Add("Amanda Bynes");
	tc.Add("Lindsay Lohan");
	tc.Add("Kathy Griffin");}

	{auto& tc = list.Add("Journalist");
	tc.Add("Ann Coulter");
	tc.Add("Roseanne Barr");
	tc.Add("Chelsea Handler");
	tc.Add("Wendy Williams");
	tc.Add("Kathy Griffin");}

	{auto& tc = list.Add("Writer/author");
	tc.Add("Chelsea Handler");
	tc.Add("Amanda Bynes");
	tc.Add("Azealia Banks");
	tc.Add("Lindsay Lohan");
	tc.Add("Roseanne Barr");}

	{auto& tc = list.Add("Celebrity/entertainer");
	tc.Add("Kim Kardashian-West");
	tc.Add("Amanda Bynes");
	tc.Add("Lindsay Lohan");
	tc.Add("Roseanne Barr");
	tc.Add("Kylie Jenner");}

	{auto& tc = list.Add("Blogger");
	tc.Add("Amanda Bynes");
	tc.Add("Roseanne Barr");
	tc.Add("Azealia Banks");
	tc.Add("Lindsay Lohan");
	tc.Add("Kathy Griffin");}

	{auto& tc = list.Add("Entrepreneur");
	tc.Add("Kylie Jenner");
	tc.Add("Kim Kardashian-West");
	tc.Add("Amanda Bynes");
	tc.Add("Lindsay Lohan");
	tc.Add("Azealia Banks");}

	{auto& tc = list.Add("Educator");
	tc.Add("Roseanne Barr");
	tc.Add("Chelsea Handler");
	tc.Add("Amanda Bynes");
	tc.Add("Azealia Banks");
	tc.Add("Ann Coulter");}

	{auto& tc = list.Add("Student");
	tc.Add("Amanda Bynes");
	tc.Add("Lindsay Lohan");
	tc.Add("Azealia Banks");
	tc.Add("Roseanne Barr");
	tc.Add("Kim Kardashian-West");}

	{auto& tc = list.Add("Parent");
	tc.Add("Kim Kardashian-West");
	tc.Add("Amanda Bynes");
	tc.Add("Lindsay Lohan");
	tc.Add("Roseanne Barr");
	tc.Add("Azealia Banks");}

	{auto& tc = list.Add("Athlete");
	tc.Add("Kim Kardashian-West");
	tc.Add("Lindsay Lohan");
	tc.Add("Amanda Bynes");
	tc.Add("Roseanne Barr");
	tc.Add("Azealia Banks");}

	{auto& tc = list.Add("Music fan");
	tc.Add("Azealia Banks");
	tc.Add("Kim Kardashian-West");
	tc.Add("Lindsay Lohan");
	tc.Add("Amanda Bynes");
	tc.Add("Wendy Williams");}

	{auto& tc = list.Add("Foodie");
	tc.Add("Amanda Bynes");
	tc.Add("Kylie Jenner");
	tc.Add("Lindsay Lohan");
	tc.Add("Roseanne Barr");
	tc.Add("Azealia Banks");}

	{auto& tc = list.Add("Traveler");
	tc.Add("Amanda Bynes");
	tc.Add("Lindsay Lohan");
	tc.Add("Kim Kardashian-West");
	tc.Add("Roseanne Barr");
	tc.Add("Azealia Banks");}

	{auto& tc = list.Add("Developer/programmer");
	tc.Add("Chelsea Handler");
	tc.Add("Amanda Bynes");
	tc.Add("Azealia Banks");
	tc.Add("Roseanne Barr");
	tc.Add("Kim Kardashian-West");}

	{auto& tc = list.Add("Entity/creative");
	tc.Add("Amanda Bynes");
	tc.Add("Lindsay Lohan");
	tc.Add("Azealia Banks");
	tc.Add("Roseanne Barr");
	tc.Add("Kim Kardashian-West");}

	{auto& tc = list.Add("Scientist/researcher");
	tc.Add("Ann Coulter");
	tc.Add("Roseanne Barr");
	tc.Add("Chelsea Handler");
	tc.Add("Wendy Williams");
	tc.Add("Amanda Bynes");}

	{auto& tc = list.Add("Environmentalist");
	tc.Add("Roseanne Barr");
	tc.Add("Chelsea Handler");
	tc.Add("Amanda Bynes");
	tc.Add("Azealia Banks");
	tc.Add("Lindsay Lohan");}

	{auto& tc = list.Add("Animal lover/activist");
	tc.Add("Amanda Bynes");
	tc.Add("Chelsea Handler");
	tc.Add("Azealia Banks");
	tc.Add("Roseanne Barr");
	tc.Add("Kim Kardashian-West");}

	{auto& tc = list.Add("Fashionista");
	tc.Add("Kylie Jenner");
	tc.Add("Kim Kardashian-West");
	tc.Add("Lindsay Lohan");
	tc.Add("Amanda Bynes");
	tc.Add("Roseanne Barr");}

	{auto& tc = list.Add("Homemaker");
	tc.Add("Roseanne Barr");
	tc.Add("Azealia Banks");
	tc.Add("Amanda Bynes");
	tc.Add("Lindsay Lohan");
	tc.Add("Kim Kardashian-West");}

	{auto& tc = list.Add("Philanthropist");
	tc.Add("Azealia Banks");
	tc.Add("Amanda Bynes");
	tc.Add("Lindsay Lohan");
	tc.Add("Roseanne Barr");
	tc.Add("Kathy Griffin");}

	{auto& tc = list.Add("Socialite");
	tc.Add("Amanda Bynes");
	tc.Add("Lindsay Lohan");
	tc.Add("Kim Kardashian-West");
	tc.Add("Roseanne Barr");
	tc.Add("Azealia Banks");}

	{auto& tc = list.Add("Food/drink critic");
	tc.Add("Amanda Bynes");
	tc.Add("Azealia Banks");
	tc.Add("Kim Kardashian-West");
	tc.Add("Lindsay Lohan");
	tc.Add("Roseanne Barr");}

	{auto& tc = list.Add("Gamer");
	tc.Add("Azealia Banks");
	tc.Add("Amanda Bynes");
	tc.Add("Lindsay Lohan");
	tc.Add("Wendy Williams");
	tc.Add("Kim Kardashian-West");}

	{auto& tc = list.Add("Fitness enthusiast");
	tc.Add("Amanda Bynes");
	tc.Add("Azealia Banks");
	tc.Add("Lindsay Lohan");
	tc.Add("Kim Kardashian-West");
	tc.Add("Rosie O'Donnell");}

	{auto& tc = list.Add("Health/wellness guru");
	tc.Add("Azealia Banks");
	tc.Add("Amanda Bynes");
	tc.Add("Lindsay Lohan");
	tc.Add("Kim Kardashian-West");
	tc.Add("Roseanne Barr");}

	{auto& tc = list.Add("Spiritual leader");
	tc.Add("Azealia Banks");
	tc.Add("Amanda Bynes");
	tc.Add("Lindsay Lohan");
	tc.Add("Kim Kardashian-West");
	tc.Add("Roseanne Barr");}

	{auto& tc = list.Add("Parenting advice");
	tc.Add("Amanda Bynes");
	tc.Add("Chelsea Handler");
	tc.Add("Azealia Banks");
	tc.Add("Lindsay Lohan");
	tc.Add("Kim Kardashian-West");}

	{auto& tc = list.Add("Career coach/advisor");
	tc.Add("Azealia Banks");
	tc.Add("Amanda Bynes");
	tc.Add("Kim Kardashian-West");
	tc.Add("Chelsea Handler");
	tc.Add("Roseanne Barr");}

	{auto& tc = list.Add("Travel blogger");
	tc.Add("Amanda Bynes");
	tc.Add("Lindsay Lohan");
	tc.Add("Roseanne Barr");
	tc.Add("Kim Kardashian-West");
	tc.Add("Azealia Banks");}

	{auto& tc = list.Add("Book lover/reader");
	tc.Add("Azealia Banks");
	tc.Add("Amanda Bynes");
	tc.Add("Lindsay Lohan");
	tc.Add("Kim Kardashian-West");
	tc.Add("Roseanne Barr");}

	{auto& tc = list.Add("DIY enthusiast");
	tc.Add("Amanda Bynes");
	tc.Add("Chelsea Handler");
	tc.Add("Kim Kardashian-West");
	tc.Add("Azealia Banks");
	tc.Add("Lindsay Lohan");}

	{auto& tc = list.Add("Pet lover/owner");
	tc.Add("Amanda Bynes");
	tc.Add("Lindsay Lohan");
	tc.Add("Kim Kardashian-West");
	tc.Add("Azealia Banks");
	tc.Add("Chelsea Handler");}

	{auto& tc = list.Add("Movie/TV critic");
	tc.Add("Roseanne Barr");
	tc.Add("Chelsea Handler");
	tc.Add("Azealia Banks");
	tc.Add("Amanda Bynes");
	tc.Add("Lindsay Lohan");}

	{auto& tc = list.Add("Beauty/fashion blogger");
	tc.Add("Kylie Jenner");
	tc.Add("Kim Kardashian-West");
	tc.Add("Roseanne Barr");
	tc.Add("Azealia Banks");
	tc.Add("Amanda Bynes");}

	{auto& tc = list.Add("Tech geek");
	tc.Add("Amanda Bynes");
	tc.Add("Lindsay Lohan");
	tc.Add("Kim Kardashian-West");
	tc.Add("Chelsea Handler");
	tc.Add("Roseanne Barr");}

	{auto& tc = list.Add("Nature lover");
	tc.Add("Azealia Banks");
	tc.Add("Amanda Bynes");
	tc.Add("Lindsay Lohan");
	tc.Add("Kim Kardashian-West");
	tc.Add("Chelsea Handler");}

	{auto& tc = list.Add("Political commentator");
	tc.Add("Ann Coulter");
	tc.Add("Roseanne Barr");
	tc.Add("Chelsea Handler");
	tc.Add("Azealia Banks");
	tc.Add("Amanda Bynes");}

	{auto& tc = list.Add("Relationship expert");
	tc.Add("Azealia Banks");
	tc.Add("Amanda Bynes");
	tc.Add("Kim Kardashian-West");
	tc.Add("Roseanne Barr");
	tc.Add("Chelsea Handler");}

	{auto& tc = list.Add("Human rights activist");
	tc.Add("Roseanne Barr");
	tc.Add("Azealia Banks");
	tc.Add("Amanda Bynes");
	tc.Add("Chelsea Handler");
	tc.Add("Kim Kardashian-West");}

	{auto& tc = list.Add("Social justice warrior");
	tc.Add("Azealia Banks");
	tc.Add("Amanda Bynes");
	tc.Add("Roseanne Barr");
	tc.Add("Chelsea Handler");
	tc.Add("Lindsay Lohan");}

	{auto& tc = list.Add("Music reviewer");
	tc.Add("Amanda Bynes");
	tc.Add("Azealia Banks");
	tc.Add("Roseanne Barr");
	tc.Add("Lindsay Lohan");
	tc.Add("Kim Kardashian-West");}

	{auto& tc = list.Add("Interior design enthusiast");
	tc.Add("Amanda Bynes");
	tc.Add("Roseanne Barr");
	tc.Add("Kim Kardashian-West");
	tc.Add("Azealia Banks");
	tc.Add("Lindsay Lohan");}

	{auto& tc = list.Add("Self-help guru");
	tc.Add("Azealia Banks");
	tc.Add("Chelsea Handler");
	tc.Add("Amanda Bynes");
	tc.Add("Kim Kardashian-West");
	tc.Add("Lindsay Lohan");}

	{auto& tc = list.Add("Life coach");
	tc.Add("Azealia Banks");
	tc.Add("Amanda Bynes");
	tc.Add("Roseanne Barr");
	tc.Add("Chelsea Handler");
	tc.Add("Kim Kardashian-West");}

	{auto& tc = list.Add("Mental health advocate");
	tc.Add("Azealia Banks");
	tc.Add("Amanda Bynes");
	tc.Add("Roseanne Barr");
	tc.Add("Chelsea Handler");
	tc.Add("Lindsay Lohan");}

	{auto& tc = list.Add("Promoter/event organizer");
	tc.Add("Roseanne Barr");
	tc.Add("Azealia Banks");
	tc.Add("Amanda Bynes");
	tc.Add("Chelsea Handler");
	tc.Add("Kim Kardashian-West");}

	{auto& tc = list.Add("Financial advisor");
	tc.Add("Azealia Banks");
	tc.Add("Roseanne Barr");
	tc.Add("Chelsea Handler");
	tc.Add("Amanda Bynes");
	tc.Add("Kim Kardashian-West");}

	{auto& tc = list.Add("Food blogger");
	tc.Add("Amanda Bynes");
	tc.Add("Lindsay Lohan");
	tc.Add("Kim Kardashian-West");
	tc.Add("Azealia Banks");
	tc.Add("Roseanne Barr");}

	{auto& tc = list.Add("Sports enthusiast");
	tc.Add("Kim Kardashian-West");
	tc.Add("Azealia Banks");
	tc.Add("Amanda Bynes");
	tc.Add("Lindsay Lohan");
	tc.Add("Roseanne Barr");}

	{auto& tc = list.Add("Fashion designer");
	tc.Add("Kim Kardashian-West");
	tc.Add("Kylie Jenner");
	tc.Add("Amanda Bynes");
	tc.Add("Lindsay Lohan");
	tc.Add("Roseanne Barr");}

	{auto& tc = list.Add("Makeup artist");
	tc.Add("Kylie Jenner");
	tc.Add("Kim Kardashian-West");
	tc.Add("Amanda Bynes");
	tc.Add("Lindsay Lohan");
	tc.Add("Roseanne Barr");}

	{auto& tc = list.Add("Gardening enthusiast");
	tc.Add("Lindsay Lohan");
	tc.Add("Kim Kardashian-West");
	tc.Add("Azealia Banks");
	tc.Add("Amanda Bynes");
	tc.Add("Roseanne Barr");}

	{auto& tc = list.Add("Geek/nerd");
	tc.Add("Azealia Banks");
	tc.Add("Amanda Bynes");
	tc.Add("Chelsea Handler");
	tc.Add("Kim Kardashian-West");
	tc.Add("Lindsay Lohan");}

	{auto& tc = list.Add("History buff");
	tc.Add("Roseanne Barr");
	tc.Add("Amanda Bynes");
	tc.Add("Chelsea Handler");
	tc.Add("Azealia Banks");
	tc.Add("Kim Kardashian-West");}

	{auto& tc = list.Add("Business owner");
	tc.Add("Kim Kardashian-West");
	tc.Add("Amanda Bynes");
	tc.Add("Azealia Banks");
	tc.Add("Lindsay Lohan");
	tc.Add("Roseanne Barr");}

	{auto& tc = list.Add("Legal expert");
	tc.Add("Azealia Banks");
	tc.Add("Amanda Bynes");
	tc.Add("Roseanne Barr");
	tc.Add("Chelsea Handler");
	tc.Add("Kim Kardashian-West");}

	{auto& tc = list.Add("Parenting blogger");
	tc.Add("Kim Kardashian-West");
	tc.Add("Amanda Bynes");
	tc.Add("Lindsay Lohan");
	tc.Add("Azealia Banks");
	tc.Add("Roseanne Barr");}

	{auto& tc = list.Add("Senior citizen/retiree");
	tc.Add("Azealia Banks");
	tc.Add("Chelsea Handler");
	tc.Add("Amanda Bynes");
	tc.Add("Roseanne Barr");
	tc.Add("Ann Coulter");}

	{auto& tc = list.Add("Marriage counselor");
	tc.Add("Amanda Bynes");
	tc.Add("Azealia Banks");
	tc.Add("Lindsay Lohan");
	tc.Add("Kim Kardashian-West");
	tc.Add("Chelsea Handler");}

	{auto& tc = list.Add("Wine connoisseur");
	tc.Add("Amanda Bynes");
	tc.Add("Lindsay Lohan");
	tc.Add("Azealia Banks");
	tc.Add("Roseanne Barr");
	tc.Add("Kim Kardashian-West");}

	{auto& tc = list.Add("Youth advocate");
	tc.Add("Azealia Banks");
	tc.Add("Chelsea Handler");
	tc.Add("Kim Kardashian-West");
	tc.Add("Amanda Bynes");
	tc.Add("Lindsay Lohan");}

	{auto& tc = list.Add("Success coach");
	tc.Add("Azealia Banks");
	tc.Add("Amanda Bynes");
	tc.Add("Chelsea Handler");
	tc.Add("Kim Kardashian-West");
	tc.Add("Lindsay Lohan");}

	{auto& tc = list.Add("Career woman/man");
	tc.Add("Azealia Banks");
	tc.Add("Amanda Bynes");
	tc.Add("Roseanne Barr");
	tc.Add("Kim Kardashian-West");
	tc.Add("Lindsay Lohan");}

	{auto& tc = list.Add("Fitness coach");
	tc.Add("Azealia Banks");
	tc.Add("Amanda Bynes");
	tc.Add("Roseanne Barr");
	tc.Add("Chelsea Handler");
	tc.Add("Kim Kardashian-West");}

	{auto& tc = list.Add("Political blogger");
	tc.Add("Azealia Banks");
	tc.Add("Amanda Bynes");
	tc.Add("Lindsay Lohan");
	tc.Add("Chelsea Handler");
	tc.Add("Kim Kardashian-West");}

	{auto& tc = list.Add("Blogger/influencer relations");
	tc.Add("Azealia Banks");
	tc.Add("Amanda Bynes");
	tc.Add("Roseanne Barr");
	tc.Add("Kim Kardashian-West");
	tc.Add("Lindsay Lohan");}

	{auto& tc = list.Add("Adult entertainer");
	tc.Add("Azealia Banks");
	tc.Add("Jonathan Cheban");
	tc.Add("Kim Kardashian-West");
	tc.Add("Lindsay Lohan");
	tc.Add("Amanda Bynes");}

	{auto& tc = list.Add("Adult content creator");
	tc.Add("Kim Kardashian-West");
	tc.Add("Amanda Bynes");
	tc.Add("Azealia Banks");
	tc.Add("Lindsay Lohan");
	tc.Add("Joe Rogan");}

	{auto& tc = list.Add("Adult industry critic");
	tc.Add("Azealia Banks");
	tc.Add("Lindsay Lohan");
	tc.Add("Amanda Bynes");
	tc.Add("Kim Kardashian-West");
	tc.Add("Chelsea Handler");}

	{auto& tc = list.Add("Adult content reviewer");
	tc.Add("Amanda Bynes");
	tc.Add("Azealia Banks");
	tc.Add("Roseanne Barr");
	tc.Add("Kim Kardashian-West");
	tc.Add("Chelsea Handler");}
	
	return list;
}

VectorMap<String,Vector<String>>& GetRolesSafe(bool gender) {
	if (!gender)
		return GetRolesSafeMale();
	else
		return GetRolesSafeFemale();
}

VectorMap<String,Vector<String>>& GetRolesUnsafe(bool gender) {
	if (!gender)
		return GetRolesUnsafeMale();
	else
		return GetRolesUnsafeFemale();
}

VectorMap<String,Vector<String>>& GetRoleCompanies(bool unsafe, bool gender) {
	if (!unsafe)
		return GetRolesSafe(gender);
	else
		return GetRolesUnsafe(gender);
}

END_TEXTLIB_NAMESPACE
