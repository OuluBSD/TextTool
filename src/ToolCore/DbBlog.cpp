#include "ToolCore.h"
#include "ProtectedCommon.h"

BEGIN_TEXTLIB_NAMESPACE

const Index<String>& GetPersonas() {
	static Index<String> list;
	ONCELOCK
	if (list.IsEmpty()) {
		list.Add("Expert/guru");
		list.Add("Aspiring/learner");
		list.Add("Creative/innovative");
		list.Add("Influencer/trendsetter");
		list.Add("Relatable/real");
		list.Add("Hilarious/entertaining");
		list.Add("Inspiring/motivator");
		list.Add("Honest/truth-teller");
		list.Add("Authentic/transparent");
		list.Add("Opinionated/thought-provoking");
		list.Add("Down-to-earth/genuine");
		list.Add("Sarcastic/witty");
		list.Add("Chill/laid-back");
		list.Add("Sensible/practical");
		list.Add("Spirited/energetic");
		list.Add("Curious/adventurous");
		list.Add("Analytical/problem-solver");
		list.Add("Supportive/encouraging");
		list.Add("Bold/brave");
		list.Add("Collaborative/connector");
		list.Add("Aspirational/motivator");
		list.Add("Personal/relatable");
		list.Add("Inquisitive/thoughtful");
		list.Add("Educator/teacher");
	}
	return list;
}

const Vector<ContentType>& GetNiches() {
	thread_local static Vector<ContentType> list;
	ONCELOCK
	if (list.IsEmpty()) {
        list.Add().Set("Fashion/Beauty", "Luxury",  "Minimalism", "Streetwear");
        list.Add().Set("Food", "Healthy", "Indulgent", "Budget-friendly");
        list.Add().Set("Travel", "Adventure", "Luxury", "Off-the-Beaten Path");
        list.Add().Set("Parenting", "Helicopter", "Free-range", "Attachment");
        list.Add().Set("Lifestyle", "Minimalist", "Maximalist", "Intentional");
        list.Add().Set("DIY/Crafts", "Eco-friendly", "Budget-friendly", "Upcycling");
        list.Add().Set("Personal Development", "Self-improvement", "Self-discovery", "Self-acceptance");
        list.Add().Set("Fitness/Wellness", "Intense", "Mindful", "Playful");
        list.Add().Set("Entertainment/Pop Culture", "Mainstream", "Alternative", "Niche");
        list.Add().Set("Finance/Money Management", "Savings", "Investing", "Debt-free");
        list.Add().Set("Career/Business", "Corporate", "Entrepreneurship", "Side Hustle");
        list.Add().Set("Politics/Current Events", "Conservative", "Liberal", "Moderate");
        list.Add().Set("Health/Mental Health", "Physical", "Emotional", "Spiritual");
        list.Add().Set("Home Decor/Interior Design", "Modern", "Vintage", "Eclectic");
        list.Add().Set("Personal Finance/Investing", "Frugal", "Luxurious", "Practical");
        list.Add().Set("Self-Care/Self-Love", "Hygge", "Self-Pampering", "Mindfulness");
        list.Add().Set("Books/Literature", "Classic", "Chick-Lit", "Thriller");
        list.Add().Set("Technology/Gadgets", "High-Tech", "Low-Tech", "Eco-Friendly");
        list.Add().Set("Environmental/Sustainability", "Green Living", "Zero Waste", "Eco-Conscious");
        list.Add().Set("Relationships", "Monogamous", "Polyamorous", "Open");
        list.Add().Set("Mental Health/Wellness", "Therapy", "Holistic", "Alternative");
        list.Add().Set("Culture/Identity", "Multicultural", "Individualism", "Collectivism");
        list.Add().Set("Social Causes/Activism", "Environmental", "Human Rights", "Animal Welfare");
        list.Add().Set("Conspiracy theories/Real news", "Conspiracy", "Skepticism", "Fact-Checking");
        list.Add().Set("Party politics", "Left-Wing", "Right-Wing", "Moderate");
        list.Add().Set("Religion/Spirituality", "Organized Religion", "New Age", "Atheism");
        list.Add().Set("Music production/songwriting", "Pop", "Indie", "Hip-Hop");
        list.Add().Set("Adventure Travel/Budget Travel", "Safari", "Backpacking", "Luxury Resorts");
        list.Add().Set("Feminism/Empowerment", "Third-Wave", "Intersectional", "Radical");
        list.Add().Set("Design/Architecture", "Modern", "Gothic", "Rustic");
        list.Add().Set("Sports/Fitness", "Team Sports", "Individual Sports", "Outdoor Adventures");
        list.Add().Set("Education/teaching/tutoring", "Traditional", "Progressive", "Montessori");
        list.Add().Set("Beauty/Self-Care", "Natural", "Glam", "DIY");
        list.Add().Set("Computer Science/Programming", "Front-End", "Back-End", "Web Development");
        list.Add().Set("Fine Arts/Crafts", "Painting", "Pottery", "Knitting");
        list.Add().Set("Film/TV/Streaming", "Hollywood Blockbusters", "Indie Films", "Web Series");
        list.Add().Set("Craft beer/wine/cocktails", "Craft Beer", "Wine Tasting", "Mixology");
        list.Add().Set("Psychology/Sociology", "Behaviorism", "Psychoanalysis", "Humanistic");
        list.Add().Set("Science/Technology", "Artificial Intelligence", "Space Exploration", "Bioengineering");
        list.Add().Set("Home-renovation/DIY", "Renovation", "Crafts", "Interior Design");
        list.Add().Set("Gaming/E-Sports", "Console Gaming", "PC Gaming", "Esports");
        list.Add().Set("Mindfulness/Meditation", "Meditation", "Yoga", "Mind-Body Connection");
        list.Add().Set("Pop Culture/Entertainment", "Celebrity Gossip", "Fan Culture", "Subcultures");
        list.Add().Set("Photography/Videography", "Portrait Photography", "Documentary Filmmaking", "Drone Videography");
        list.Add().Set("Thrifting/Sustainable Fashion", "Thrifty Fashion", "Sustainable Fashion", "Upcycled Fashion");
        list.Add().Set("Pet/Animal Care", "Dog Owners", "Cat Lovers", "Bird Enthusiasts");
        list.Add().Set("Homeschooling/Unschooling", "Structured Homeschooling", "Unschooling", "Roadschooling");
        list.Add().Set("Minimalism/Decluttering", "Minimalist Living", "KonMari Method", "Digital Minimalism");
        list.Add().Set("Stock Market/Investing", "Value Investing", "Day Trading", "Cryptocurrency");
	}
	return list;
}

int GetNicheCount() {
	return GetNiches().GetCount();
}


VectorMap<String,Vector<String>>& GetPersonaSafeMale() {
	thread_local static VectorMap<String,Vector<String>> list;
	ONCELOCK
	if (!list.IsEmpty()) return list;
	
    {auto& tc = list.Add("Expert/guru");
    tc.Add("Tim Ferriss (productivity/self-improvement)");
    tc.Add("Gary Vaynerchuk (marketing/branding)");
    tc.Add("Tony Robbins (self-help/motivational)");
    tc.Add("Ryan Holiday (stoicism/life advice)");
    tc.Add("Lewis Howes (business/mindset)");
    tc.Add("Tai Lopez (entrepreneurship/investing)");
    tc.Add("Dave Asprey (biohacking/productivity)");
    tc.Add("Seth Godin (marketing/authorship) ");}

    {auto& tc = list.Add("Aspiring/learner");
    tc.Add("Matt D'Avella (minimalism/creativity)");
    tc.Add("Chris Guillebeau (travel/entrepreneurship)");
    tc.Add("Cal Newport (productivity/education)");
    tc.Add("Mark Manson (self-help/personal development)");
    tc.Add("Derek Sivers (entrepreneurship/lifestyle design)");
    tc.Add("Andrew Fiebert (finance/investing)");
    tc.Add("Chase Jarvis (photography/creativity)");
    tc.Add("Nathan Barry (writing/productivity)");}

    {auto& tc = list.Add("Creative/innovative");
    tc.Add("David Chang (food/restaurant industry)");
    tc.Add("Austin Kleon (writing/creativity)");
    tc.Add("Hugh Howey (writing/self-publishing)");
    tc.Add("Casey Neistat (film/vlogging)");
    tc.Add("Jason Silva (artificial intelligence/philosophy)");
    tc.Add("Jamie Oliver (food/health)");
    tc.Add("Marcus Samuelsson (food/culinary entrepreneurship)");
    tc.Add("David Sedaris (humor/writing)");}

    {auto& tc = list.Add("Influencer/trendsetter");
    tc.Add("Brian Kelly (travel/rewards points)");
    tc.Add("Jeffree Star (beauty/makeup)");
    tc.Add("Casey Neistat (film/vlogging)");
    tc.Add("Gary Vaynerchuk (marketing/branding)");
    tc.Add("Tony Robbins (self-help/motivational)");
    tc.Add("Scott Disick (fashion/lifestyle)");
    tc.Add("James Charles (beauty/makeup)");
    tc.Add("Jeremy Fragrance (fragrances/grooming)");}

    {auto& tc = list.Add("Relatable/real");
    tc.Add("Pete Holmes (comedy/relationships)");
    tc.Add("Conan O'Brien (comedy/entertainment)");
    tc.Add("Judd Apatow (comedy/fiction)");
    tc.Add("Aziz Ansari (comedy/relationships)");
    tc.Add("Trevor Noah (comedy/politics)");
    tc.Add("Steve Martin (comedy/writing)");
    tc.Add("Jon Favreau (cooking/food)");
    tc.Add("Joe Rogan (comedy/podcasting)");}

    {auto& tc = list.Add("Hilarious/entertaining");
    tc.Add("Kevin Hart (comedy/motivational)");
    tc.Add("Bo Burnham (comedy/music)");
    tc.Add("Dave Chappelle (comedy/politics)");
    tc.Add("Ali Wong (comedy/parenting)");
    tc.Add("Seth Rogen (comedy/film)");
    tc.Add("Conan O'Brien (comedy/entertainment)");
    tc.Add("Ryan Reynolds (comedy/acting)");
    tc.Add("Russell Brand (comedy/spirituality)");}

    {auto& tc = list.Add("Inspiring/motivator");
    tc.Add("Tony Robbins (self-help/motivational)");
    tc.Add("Mel Robbins (self-help/productivity)");
    tc.Add("Gabby Bernstein (self-help/spirituality)");
    tc.Add("Gary Vaynerchuk (marketing/branding)");
    tc.Add("Tim Ferriss (productivity/self-improvement)");
    tc.Add("Lewis Howes (business/mindset)");}

    {auto& tc = list.Add("Honest/truth-teller");
    tc.Add("James Clear (productivity/habit formation)");
    tc.Add("Seth Godin (marketing/entrepreneurship)");
    tc.Add("Gary Vaynerchuk (entrepreneurship/marketing)");
    tc.Add("Mark Manson (self-help/personal development)");
    tc.Add("Simon Sinek (leadership/management)");
    tc.Add("Tim Ferriss (lifestyle design/productivity)");
    tc.Add("Ryan Holiday (philosophy/stoicism)");
    tc.Add("Ben Shapiro (political commentator/commentary)");}

    {auto& tc = list.Add("Authentic/transparent");
    tc.Add("James Clear (productivity/habit formation)");
    tc.Add("Mark Manson (self-help/personal development)");
    tc.Add("Gary Vaynerchuk (entrepreneurship/marketing)");
    tc.Add("Ramit Sethi (finance/personal development)");
    tc.Add("Seth Godin (marketing/entrepreneurship)");
    tc.Add("Tim Ferriss (lifestyle design/productivity)");
    tc.Add("Chase Jarvis (photography/entrepreneurship)");
    tc.Add("Casey Neistat (filmmaking/entrepreneurship)");}

    {auto& tc = list.Add("Opinionated/thought-provoking");
    tc.Add("Tim Urban (psychology/productivity)");
    tc.Add("Malcolm Gladwell (sociology/authorship)");
    tc.Add("Jordan Peterson (philosophy/psychology)");
    tc.Add("Ben Shapiro (politics/current events)");
    tc.Add("Seth Godin (marketing/authorship)");
    tc.Add("Trevor Noah (comedy/politics)");
    tc.Add("Dave Rubin (media/interviews)");
    tc.Add("Tucker Carlson (journalism/opinions)");}

    {auto& tc = list.Add("Down-to-earth/genuine");
    tc.Add("Jocko Willink (fitness/motivation)");
    tc.Add("Mark Manson (personal development/relationships)");
    tc.Add("James Clear (productivity/habit formation)");
    tc.Add("Matt D'Avella (minimalism/documentary)");
    tc.Add("Casey Neistat (vlogging/creativity)");
    tc.Add("John Green (young adult author/YouTuber)");
    tc.Add("David Sedaris (humor/essays)");
    tc.Add("Jon Acuff (career/entrepreneurship)");}

    {auto& tc = list.Add("Sarcastic/witty");
    tc.Add("Ryan Reynolds (celebrity/entertainment)");
    tc.Add("Conan O'Brien (late night comedy)");
    tc.Add("John Oliver (satirical news/politics)");
    tc.Add("Dave Barry (humor/columnist)");
    tc.Add("Andy Borowitz (satirical news/writing)");
    tc.Add("Aziz Ansari (comedy/relationships)");
    tc.Add("Stephen Fry (comedian/author)");
    tc.Add("James Corden (late night comedy/music)");}

    {auto& tc = list.Add("Chill/laid-back");
    tc.Add("Tim Ferriss (lifestyle design/productivity)");
    tc.Add("Joe Rogan (comedy/podcasting)");
    tc.Add("Casey Neistat (vlogging/travel)");
    tc.Add("Gary Vaynerchuk (entrepreneurship/marketing)");
    tc.Add("Chase Jarvis (photography/creativity)");
    tc.Add("Anthony Bourdain (food/travel)");
    tc.Add("Rhett & Link (comedy/sketches)");
    tc.Add("Jim Gaffigan (comedy/food)");}

    {auto& tc = list.Add("Sensible/practical");
    tc.Add("Dave Ramsey (finance/money management)");
    tc.Add("Adam Grant (business/organizational psychology)");
    tc.Add("Thomas Frank (productivity/education)");
    tc.Add("Tim Ferriss (lifestyle design/productivity)");
    tc.Add("Tony Robbins (personal development/motivation)");
    tc.Add("SmarterEveryDay (science/education)");
    tc.Add("Ryan Holiday (media/philosophy)");
    tc.Add("Chris Guillebeau (travel/personal development)");}

    {auto& tc = list.Add("Spirited/energetic");
    tc.Add("Tony Robbins (personal development/motivation)");
    tc.Add("Gary Vaynerchuk (entrepreneurship/marketing)");
    tc.Add("Lewis Howes (business/personal development)");
    tc.Add("Tim Ferriss (lifestyle design/productivity)");
    tc.Add("Brendon Burchard (self-help/motivation)");
    tc.Add("Eric Thomas (motivational speaker/inspiration)");
    tc.Add("Jack Canfield (personal development/success)");
    tc.Add("Grant Cardone (entrepreneurship/sales)");}

    {auto& tc = list.Add("Curious/adventurous");
    tc.Add("Bear Grylls (outdoors/adventure)");
    tc.Add("Anthony Bourdain (food/travel)");
    tc.Add("The Bucket List Family (travel/family adventures)");
    tc.Add("Richard Branson (entrepreneurship/adventure)");
    tc.Add("Tim Ferriss (lifestyle design/adventure)");
    tc.Add("Alex Honnold (rock climbing/adventure)");
    tc.Add("Chris Burkard (photography/adventure)");
    tc.Add("Roman Atwood (vlogging/family adventures)");}

    {auto& tc = list.Add("Analytical/problem-solver");
    tc.Add("Neil deGrasse Tyson (science/astrophysics)");
    tc.Add("Bill Nye (science/education)");
    tc.Add("Malcolm Gladwell (psychology/sociology)");
    tc.Add("Simon Sinek (leadership/management)");
    tc.Add("Daymond John (entrepreneurship/business)");
    tc.Add("Tim Ferriss (lifestyle design/productivity)");
    tc.Add("Cal Newport (computer science/productivity)");
    tc.Add("James Clear (productivity/habit formation)");}

    {auto& tc = list.Add("Supportive/encouraging");
    tc.Add("Brendon Burchard (self-help/motivation)");
    tc.Add("Tony Robbins (personal development/motivation)");
    tc.Add("Brené Brown (self-acceptance/empowerment)");
    tc.Add("Nick Vujicic (inspirational speaker/disability advocate)");
    tc.Add("Jay Shetty (personal development/philosophy)");
    tc.Add("Trent Shelton (motivational speaker/personal development)");
    tc.Add("John O'Leary (inspirational speaker/personal growth)");
    tc.Add("Mel Robbins (personal development/overcoming fears)");}

    {auto& tc = list.Add("Bold/brave");
    tc.Add("David Goggins (motivational speaker/ultra-endurance athlete)");
    tc.Add("Jocko Willink (leadership/discipline)");
    tc.Add("Gary Vaynerchuk (entrepreneurship/marketing)");
    tc.Add("Lewis Howes (personal development/podcast host)");
    tc.Add("Lewis Hamilton (athletics/formula one racing)");
    tc.Add("Casey Neistat (filmmaking/entrepreneurship)");
    tc.Add("Mark Manson (self-help/personal development)");
    tc.Add("Grant Cardone (sales/motivation)");}

    {auto& tc = list.Add("Collaborative/connector");
    tc.Add("Tim Urban (blogging/writing)");
    tc.Add("Jeff Goins (blogging/writing)");
    tc.Add("Pat Flynn (entrepreneurship/blogging)");
    tc.Add("Michael Hyatt (leadership/productivity)");
    tc.Add("Ramit Sethi (finance/personal development)");
    tc.Add("Chase Jarvis (photography/entrepreneurship)");
    tc.Add("Greg McKeown (productivity/minimalism)");
    tc.Add("Simon Sinek (leadership/management)");}

    {auto& tc = list.Add("Aspirational/motivator");
    tc.Add("Tony Robbins (personal development/motivation)");
    tc.Add("Gary Vaynerchuk (entrepreneurship/marketing)");
    tc.Add("Brendon Burchard (self-help/motivation)");
    tc.Add("Eric Thomas (motivational speaker/entrepreneur)");
    tc.Add("Grant Cardone (sales/motivation)");
    tc.Add("Robin Sharma (leadership/productivity)");
    tc.Add("Mel Robbins (personal development/overcoming fears)");
    tc.Add("Jay Shetty (personal development/philosophy)");}

    {auto& tc = list.Add("Personal/relatable' :");
    tc.Add("The Minimalists (minimalism/lifestyle design)");
    tc.Add("Tim Ferriss (lifestyle design/productivity)");
    tc.Add("Mark Manson (self-help/personal development)");
    tc.Add("Chris Guillebeau (travel/entrepreneurship)");
    tc.Add("Muneeb Ali (entrepreneurship/productivity)");
    tc.Add("Ramit Sethi (finance/personal development)");
    tc.Add("Casey Neistat (filmmaking/entrepreneurship)");
    tc.Add("Simon Sinek (leadership/management)");}

    {auto& tc = list.Add("Inquisitive/thoughtful' :");
    tc.Add("Tim Urban (blogging/writing)");
    tc.Add("Yuval Noah Harari (history/philosophy)");
    tc.Add("Sam Harris (philosophy/science)");
    tc.Add("Ryan Holiday (philosophy/stoicism)");
    tc.Add("Cal Newport (computer science/productivity)");
    tc.Add("James Clear (productivity/habit formation)");
    tc.Add("Malcolm Gladwell (psychology/sociology)");
    tc.Add("Mark Manson (self-help/personal development)");}

    {auto& tc = list.Add("Educator/teacher");
    tc.Add("Seth Godin (marketing/entrepreneurship)");
    tc.Add("Chris Guillebeau (travel/entrepreneurship)");
    tc.Add("Pat Flynn (entrepreneurship/blogging)");
    tc.Add("James Altucher (entrepreneurship/writing)");
    tc.Add("Ray Dalio (finance/investing)");
    tc.Add("Simon Sinek (leadership/management)");
    tc.Add("Ramit Sethi (finance/personal development)");
    tc.Add("Tim Ferriss (lifestyle design/productivity) ");
    tc.Add("John Maxwell (leadership/influence)");
    tc.Add("Bo Eason (storytelling/performance)");}
	ASSERT(GetPersonas().GetCount() == list.GetCount());
	return list;
}

VectorMap<String,Vector<String>>& GetPersonaSafeFemale() {
	thread_local static VectorMap<String,Vector<String>> list;
	ONCELOCK
	if (!list.IsEmpty()) return list;
    {auto& tc = list.Add("Expert/guru");
    tc.Add("Marie Forleo (entrepreneurship/personal development)");
    tc.Add("Rachel Hollis (personal development/entrepreneurship)");
    tc.Add("Amanda Kloots (fitness/health)");
    tc.Add("Mel Robbins (self-improvement/motivation)");
    tc.Add("Gretchen Rubin (happiness/habits)");
    tc.Add("Jen Sincero (finance/self-help)");
    tc.Add("Amy Porterfield (online marketing/business)");
    tc.Add("Sophia Amoruso (fashion/entrepreneurship)");}

    {auto& tc = list.Add("Aspiring/learner");
    tc.Add("Jen Hatmaker (faith/personal development)");
    tc.Add("Luvvie Ajayi (humor/speaking)");
    tc.Add("Abby Wambach (sports/leadership)");
    tc.Add("Rupi Kaur (poetry/writing)");
    tc.Add("Mimi Ikonn (lifestyle/design)");
    tc.Add("Michelle Schroeder-Gardner (finance/blogging)");
    tc.Add("Jenna Kutcher (photography/marketing)");
    tc.Add("Rachel Brathen (yoga/wellness)");}

    {auto& tc = list.Add("Creative/innovative");
    tc.Add("Brene Brown (personal development/writing)");
    tc.Add("Justina Blakeney (interior design/creativity)");
    tc.Add("Joy Cho (design/branding)");
    tc.Add("Emily Henderson (interior design/home decor)");
    tc.Add("Grace Bonney (art/design/entrepreneurship)");
    tc.Add("Elsie Larson (DIY/crafts/creativity)");
    tc.Add("Susan Kare (graphic design/technology)");
    tc.Add("Gabby Bernstein (spirituality/creativity)");}

    {auto& tc = list.Add("Influencer/trendsetter");
    tc.Add("Chiara Ferragni (fashion/trends)");
    tc.Add("Aimee Song (fashion/lifestyle)");
    tc.Add("Camila Coelho (beauty/lifestyle)");
    tc.Add("Tess Holliday (body positivity/fashion)");
    tc.Add("Huda Kattan (beauty/influencer marketing)");
    tc.Add("Leandra Medine (fashion/humor)");
    tc.Add("Nikkie Tutorials (beauty/trends)");
    tc.Add("Jenn Im (fashion/lifestyle)");}

    {auto& tc = list.Add("Relatable/real");
    tc.Add("Sarah Knight (self-help/humor)");
    tc.Add("Jenny Lawson (humor/memoir)");
    tc.Add("Glennon Doyle Melton (emotional wellness/self-discovery)");
    tc.Add("Hannah Hart (humor/LGBTQ+ advocate)");
    tc.Add("Jameela Jamil (body positivity/mental health advocacy)");
    tc.Add("Luvvie Ajayi (humor/speaking)");
    tc.Add("Nora McInerny (personal essays/grief)");
    tc.Add("Felicia Day (geek culture/humor)");}
    
    {auto& tc = list.Add("Hilarious/entertaining");
    tc.Add("Colleen Ballinger (comedy/entertainment)");
    tc.Add("Grace Helbig (comedy/entertainment)");
    tc.Add("Lilly Singh (comedy/entertainment)");
    tc.Add("Mindy Kaling (comedy/writing)");
    tc.Add("Iliza Shlesinger (comedy/writing)");
    tc.Add("Phoebe Robinson (comedy/podcasting)");
    tc.Add("Hannah Hart (comedy/entertainment)");
    tc.Add("Issa Rae (comedy/writing)");}

    {auto& tc = list.Add("Inspiring/motivator");
    tc.Add("Mel Robbins (self-improvement/motivation)");
    tc.Add("Brene Brown (personal development/writing)");
    tc.Add("Elizabeth Gilbert (creativity/self-discovery)");
    tc.Add("Rachel Hollis (personal development/entrepreneurship)");
    tc.Add("Gabby Bernstein (spirituality/creativity)");
    tc.Add("Mimi Ikonn (lifestyle/design)");
    tc.Add("Jen Hatmaker (faith/personal development)");
    tc.Add("Abbey Wambach (sports/leadership)");}

    {auto& tc = list.Add("Honest/truth-teller");
    tc.Add("Glennon Doyle Melton (emotional wellness/self-discovery)");
    tc.Add("Brene Brown (personal development/writing)");
    tc.Add("Nora McInerny (personal essays/grief)");
    tc.Add("Jenna Kutcher (body positivity/self-acceptance)");
    tc.Add("Jen Hatmaker (faith/personal development)");
    tc.Add("Elizabeth Gilbert (women's issues/empowerment)");
    tc.Add("Mindy Kaling (body positivity/writing)");
    tc.Add("Jameela Jamil (body positivity/mental health advocacy)");}

    {auto& tc = list.Add("Authentic/transparent");
    tc.Add("Glennon Doyle Melton (emotional wellness/self-discovery)");
    tc.Add("Jen Hatmaker (faith/personal development)");
    tc.Add("Brené Brown (personal development/writing)");
    tc.Add("Nora McInerny (personal essays/grief)");
    tc.Add("Rachel Brathen (yoga/wellness)");
    tc.Add("Mimi Ikonn (lifestyle/design)");
    tc.Add("Jameela Jamil (body positivity/mental health advocacy)");
    tc.Add("Luvvie Ajayi (humor/speaking)");}

    {auto& tc = list.Add("Opinionated/thought-provoking");
    tc.Add("Elizabeth Gilbert (creativity/self-discovery)");
    tc.Add("Glennon Doyle Melton (emotional wellness/self-discovery)");
    tc.Add("Chimamanda Ngozi Adichie (feminism/race)");
    tc.Add("Amandla Stenberg (activism/pop culture)");
    tc.Add("Roxane Gay (feminism/writing)");
    tc.Add("Rashida Jones (feminism/lifestyle)");
    tc.Add("Shonda Rhimes (entertainment/writing)");
    tc.Add("Roxane Gay (feminism/writing)");}

    {auto& tc = list.Add("Down-to-earth/genuine");
    tc.Add("Fredricka Whitfield (journalism/honesty)");
    tc.Add("Abby Wambach (sports/leadership)");
    tc.Add("Oprah Winfrey (empowerment/authenticity)");
    tc.Add("Ellen DeGeneres (comedy/relatability)");
    tc.Add("Issa Rae (comedy/writing)");
    tc.Add("Keanu Reeves (film/philanthropy)");
    tc.Add("Paul Rudd (film/humor)");
    tc.Add("Amy Poehler (comedy/writing)");}

    {auto& tc = list.Add("Sarcastic/witty");
    tc.Add("Samantha Bee (comedy/political satire)");
    tc.Add("Mindy Kaling (body positivity/writing)");
    tc.Add("Jameela Jamil (body positivity/mental health advocacy)");
    tc.Add("Tina Fey (comedy/writing)");
    tc.Add("Iliza Shlesinger (comedy/writing)");
    tc.Add("Phoebe Robinson (comedy/podcasting)");
    tc.Add("Amy Poehler (comedy/writing)");
    tc.Add("Issa Rae (comedy/writing)");}

    {auto& tc = list.Add("Chill/laid-back");
    tc.Add("Bethenny Frankel (business/relaxed lifestyle)");
    tc.Add("Amy Schumer (comedy/normcore)");
    tc.Add("Jennifer Lawrence (film/relaxed lifestyle)");
    tc.Add("Mindy Kaling (body positivity/writing)");
    tc.Add("Issa Rae (comedy/writing)");
    tc.Add("Jameela Jamil (body positivity/mental health advocacy)");
    tc.Add("Abby Wambach (sports/leadership)");
    tc.Add("Keanu Reeves (film/philanthropy)");}

    {auto& tc = list.Add("Sensible/practical");
    tc.Add("Marie Forleo (entrepreneurship/personal development)");
    tc.Add("Mimi Ikonn (lifestyle/design)");
    tc.Add("Ramit Sethi (finance/personal development)");
    tc.Add("Pat Flynn (entrepreneurship/blogging)");
    tc.Add("Gretchen Rubin (happiness/habits)");
    tc.Add("Brene Brown (personal development/writing)");
    tc.Add("Joanna Gaines (home design/practicality)");
    tc.Add("Ruth Soukup (productivity/organization)");}

    {auto& tc = list.Add("Spirited/energetic");
    tc.Add("Tony Robbins (personal development/energy)");
    tc.Add("Jen Hatmaker (faith/personal development)");
    tc.Add("Brene Brown (personal development/writing)");
    tc.Add("Oprah Winfrey (empowerment/authenticity)");
    tc.Add("Elizabeth Gilbert (creativity/self-discovery)");
    tc.Add("Rachel Hollis (personal development/entrepreneurship)");
    tc.Add("Gabby Bernstein (spirituality/creativity)");
    tc.Add("Marie Forleo (entrepreneurship/personal development)");}

    {auto& tc = list.Add("Curious/adventurous");
    tc.Add("Elizabeth Gilbert (creativity/self-discovery)");
    tc.Add("Abby Wambach (sports/leadership)");
    tc.Add("Brené Brown (personal development/writing)");
    tc.Add("Cheryl Strayed (adventure/self-discovery)");
    tc.Add("Amanda Lindhout (journalism/adventure)");
    tc.Add("Gretchen Rubin (happiness/habits)");
    tc.Add("Shonda Rhimes (entertainment/writing)");
    tc.Add("Elizabeth Gilbert (women's issues/empowerment)");}

    {auto& tc = list.Add("Analytical/problem-solver");
    tc.Add("Malala Yousafzai (activism/problem-solving)");
    tc.Add("Melinda Gates (philanthropy/problem-solving)");
    tc.Add("Elizabeth Holmes (entrepreneurship/problem-solving)");
    tc.Add("Marissa Mayer (technology/problem-solving)");
    tc.Add("Kari Byron (science/innovation)");
    tc.Add("Sheryl Sandberg (business/leadership)");
    tc.Add("Priyanka Chopra (activism/social issues)");
    tc.Add("Melinda Gates (business/women's issues)");}

    {auto& tc = list.Add("Supportive/encouraging");
    tc.Add("Marie Forleo (entrepreneurship/personal development)");
    tc.Add("Jen Hatmaker (faith/personal development)");
    tc.Add("Brene Brown (personal development/writing)");
    tc.Add("Oprah Winfrey (empowerment/authenticity)");
    tc.Add("Elizabeth Gilbert (women's issues/empowerment)");
    tc.Add("Rachel Hollis (personal development/entrepreneurship)");
    tc.Add("Gabby Bernstein (spirituality/creativity)");
    tc.Add("Mimi Ikonn (lifestyle/design)");}

    {auto& tc = list.Add("Bold/brave");
    tc.Add("Brene Brown (personal development/writing)");
    tc.Add("Brené Brown (personal development/writing)");
    tc.Add("Malala Yousafzai (activism/problems-solving)");
    tc.Add("Abby Wambach (sports/leadership)");
    tc.Add("Amanda Lindhout (journalism/adventure)");
    tc.Add("J.K. Rowling (writing/activism)");
    tc.Add("Roxane Gay (feminism/writing)");
    tc.Add("Issa Rae (comedy/writing)");}

    {auto& tc = list.Add("Collaborative/connector");
    tc.Add("Marie Forleo (entrepreneurship/personal development)");
    tc.Add("Brene Brown (personal development/writing)");
    tc.Add("Oprah Winfrey (empowerment/authenticity)");
    tc.Add("Elizabeth Gilbert (creativity/self-discovery)");
    tc.Add("Melinda Gates (philanthropy/collaboration)");
    tc.Add("Rachel Hollis (personal development/entrepreneurship)");
    tc.Add("Sheryl Sandberg (business/leadership)");
    tc.Add("Amy Poehler (comedy/activism)");}

    {auto& tc = list.Add("Aspirational/motivator");
    tc.Add("Tony Robbins (personal development/motivation)");
    tc.Add("Oprah Winfrey (empowerment/authenticity)");
    tc.Add("Elizabeth Gilbert (creativity/self-discovery)");
    tc.Add("Jen Hatmaker (faith/personal development)");
    tc.Add("Rachel Hollis (personal development/entrepreneurship)");
    tc.Add("Gabrielle Bernstein (spirituality/creativity)");
    tc.Add("Marie Forleo (entrepreneurship/personal development)");
    tc.Add("Mimi Ikonn (lifestyle/design)");}

    {auto& tc = list.Add("Personal/relatable");
    tc.Add("Marie Forleo (entrepreneurship/personal development)");
    tc.Add("Brene Brown (personal development/writing)");
    tc.Add("Oprah Winfrey (empowerment/authenticity)");
    tc.Add("Elizabeth Gilbert (creativity/self-discovery)");
    tc.Add("Rachel Hollis (personal development/entrepreneurship)");
    tc.Add("Abby Wambach (sports/leadership)");
    tc.Add("Amy Poehler (comedy/friendship)");
    tc.Add("Jen Hatmaker (faith/personal development)");}

    {auto& tc = list.Add("Inquisitive/thoughtful");
    tc.Add("Brené Brown (personal development/writing)");
    tc.Add("Brene Brown (personal development/writing)");
    tc.Add("Elizabeth Gilbert (creativity/self-discovery)");
    tc.Add("Brene Brown (personal development/writing)");
    tc.Add("Jen Hatmaker (faith/personal development)");
    tc.Add("Glennon Doyle Melton (emotional wellness/self-discovery)");
    tc.Add("Negin Farsad (comedy/politics)");
    tc.Add("Shonda Rhimes (entertainment/writing)");}

    {auto& tc = list.Add("Educator/teacher");
    tc.Add("Brene Brown (personal development/writing)");
    tc.Add("Rachel Hollis (personal development/entrepreneurship)");
    tc.Add("Amanda Lindhout (journalism/adventure)");
    tc.Add("Glennon Doyle Melton (emotional wellness/self-discovery)");
    tc.Add("Jen Hatmaker (faith/personal development)");
    tc.Add("Elizabeth Gilbert (creativity/self-discovery)");
    tc.Add("Oprah Winfrey (empowerment/authenticity)");
    tc.Add("Abby Wambach (sports/leadership)");}
	
	ASSERT(GetPersonas().GetCount() == list.GetCount());
	return list;
}

VectorMap<String,Vector<String>>& GetPersonaUnsafeMale() {
	thread_local static VectorMap<String,Vector<String>> list;
	ONCELOCK
	if (!list.IsEmpty()) return list;
	
    {auto& tc = list.Add("Expert/guru");
    tc.Add("Dave Asprey (health/wellness)");
    tc.Add("Gary Vaynerchuk (entrepreneurship/marketing)");
    tc.Add("Tony Robbins (personal development/coaching)");
    tc.Add("Grant Cardone (sales/entrepreneurship)");
    tc.Add("Tai Lopez (entrepreneurship/self-improvement)");}

    {auto& tc = list.Add("Aspiring/learner");
    tc.Add("Tim Urban (self-improvement/curiosity)");
    tc.Add("Jocko Willink (leadership/discipline)");
    tc.Add("Lewis Howes (personal development/sports)");
    tc.Add("Richard Branson (entrepreneurship/adventure)");
    tc.Add("Ryan Holiday (self-improvement/philosophy)");}

    {auto& tc = list.Add("Creative/innovative");
    tc.Add("Casey Neistat (filmmaking/entrepreneurship)");
    tc.Add("Jesse Driftwood (filmmaking/photography)");
    tc.Add("Austin Kleon (writing/creativity)");
    tc.Add("Steve Jobs (technology/innovation)");
    tc.Add("Elon Musk (technology/innovation)");}

    {auto& tc = list.Add("Influencer/trendsetter");
    tc.Add("James Charles (beauty/influencer)");
    tc.Add("Aja Dang (fashion/beauty/lifestyle)");
    tc.Add("Chiara Ferragni (fashion/influencer)");
    tc.Add("Joe Wicks (fitness/health/food)");
    tc.Add("Joshua Weissman (food/cooking)");}

    {auto& tc = list.Add("Relatable/real");
    tc.Add("James Clear (self-improvement/productivity)");
    tc.Add("Humble The Poet (poetry/self-reflection)");
    tc.Add("Tim Ferriss (self-experimentation/personal development)");
    tc.Add("Kevin O'Leary (entrepreneurship/personal finance)");
    tc.Add("Casey Neistat (filmmaking/lifestyle)");}

    {auto& tc = list.Add("Hilarious/entertaining");
    tc.Add("Kevin Hart (comedy/entertainment)");
    tc.Add("Trevor Noah (comedy/commentary)");
    tc.Add("The Try Guys (comedy/lifestyle)");
    tc.Add("Brodie Smith (sports/entertainment)");
    tc.Add("John Oliver (comedy/commentary)");}

    {auto& tc = list.Add("Inspiring/motivator");
    tc.Add("Les Brown (motivational speaking/coaching)");
    tc.Add("Inky Johnson (motivational speaking/leadership)");
    tc.Add("David Goggins (mental endurance/motivation)");
    tc.Add("Eric Thomas (motivational speaking/sports)");
    tc.Add("Nick Vujicic (motivational speaking/disability awareness)");}

    {auto& tc = list.Add("Honest/truth-teller");
    tc.Add("Jordan Peterson (psychology/philosophy)");
    tc.Add("Ryan Holiday (self-improvement/philosophy)");
    tc.Add("Sam Harris (philosophy/science)");
    tc.Add("Naval Ravikant (entrepreneurship/philosophy)");
    tc.Add("Joe Rogan (comedy/sports/current events)");}

    {auto& tc = list.Add("Authentic/transparent");
    tc.Add("Brene Brown (vulnerability/research)");
    tc.Add("Simon Sinek (leadership/communication)");
    tc.Add("Lewis Howes (personal development/sports)");
    tc.Add("Casey Neistat (filmmaking/lifestyle)");
    tc.Add("Gary Vaynerchuk (social media/marketing)");}

    {auto& tc = list.Add("Opinionated/thought-provoking");
    tc.Add("Ben Shapiro (political commentary/conservatism)");
    tc.Add("Malcolm Gladwell (social science/writing)");
    tc.Add("Jordan Peterson (psychology/philosophy)");
    tc.Add("Sam Harris (philosophy/science)");
    tc.Add("Dan Lok (entrepreneurship/business)");}

    {auto& tc = list.Add("Down-to-earth/genuine");
    tc.Add("Matthew Hussey (dating/relationships)");
    tc.Add("David Goggins (mental endurance/motivation)");
    tc.Add("Jocko Willink (leadership/discipline)");
    tc.Add("Jaco de Bruyn (fitness/health)");
    tc.Add("James Clear (self-improvement/productivity)");}

    {auto& tc = list.Add("Sarcastic/witty");
    tc.Add("John Oliver (comedy/commentary)");
    tc.Add("Hasan Minhaj (comedy/commentary)");
    tc.Add("Dave Chappelle (comedy/social commentary)");
    tc.Add("Trevor Noah (comedy/commentary)");
    tc.Add("George Carlin (comedy/social commentary)");}

    {auto& tc = list.Add("Chill/laid-back");
    tc.Add("Casey Neistat (filmmaking/lifestyle)");
    tc.Add("The Rock (fitness/entertainment)");
    tc.Add("Alfie Deyes (vlogging/lifestyle)");
    tc.Add("Mark Wiens (travel/food)");
    tc.Add("Kien Lam (travel/photography)");}

    {auto& tc = list.Add("Sensible/practical");
    tc.Add("Dave Ramsey (personal finance/debt-free living)");
    tc.Add("Eric Ries (entrepreneurship/lean startup)");
    tc.Add("Tim Ferriss (self-experimentation/productivity)");
    tc.Add("Mark Manson (self-improvement/relationships)");
    tc.Add("Naval Ravikant (entrepreneurship/philosophy/biography)");}

    {auto& tc = list.Add("Spirited/energetic");
    tc.Add("Tai Lopez (entrepreneurship/self-improvement)");
    tc.Add("Jocko Willink (leadership/discipline)");
    tc.Add("Tony Robbins (motivational speaking/coaching)");
    tc.Add("Les Brown (motivational speaking/motivation)");
    tc.Add("Gary Vaynerchuk (social media/marketing)");}

    {auto& tc = list.Add("Curious/adventurous");
    tc.Add("Bear Grylls (outdoor/adventure)");
    tc.Add("Elon Musk (technology/innovation)");
    tc.Add("David Goggins (mental endurance/motivation)");
    tc.Add("Richard Branson (entrepreneurship/adventure)");
    tc.Add("Tim Ferriss (adventure/productivity)");}

    {auto& tc = list.Add("Analytical/problem-solver");
    tc.Add("Ray Dalio (investing/economics)");
    tc.Add("Bill Gates (technology/philanthropy)");
    tc.Add("Neil deGrasse Tyson (science/astrophysics)");
    tc.Add("Jordan Peterson (psychology/philosophy)");
    tc.Add("Aaron Swartz (technology/activism)");}

    {auto& tc = list.Add("Supportive/encouraging");
    tc.Add("Eric Thomas (motivational speaking/sports)");
    tc.Add("Inky Johnson (motivational speaking/leadership)");
    tc.Add("Brene Brown (vulnerability/research)");
    tc.Add("Tim Ferriss (self-experimentation/productivity)");
    tc.Add("Dave Ramsey (personal finance/debt-free living)");}

    {auto& tc = list.Add("Bold/brave");
    tc.Add("Tim Ferriss (self-experimentation/productivity)");
    tc.Add("David Goggins (mental endurance/motivation)");
    tc.Add("Malcolm Gladwell (social science/writing)");
    tc.Add("Joe Rogan (comedy/sports/current events)");
    tc.Add("Elon Musk (technology/innovation)");}

    {auto& tc = list.Add("Collaborative/connector");
    tc.Add("Tim Ferriss (self-experimentation/productivity)");
    tc.Add("Tony Robbins (motivational speaking/coaching)");
    tc.Add("Lewis Howes (personal development/sports)");
    tc.Add("Larry Page (technology/innovation)");
    tc.Add("Peter Thiel (venture capital/entrepreneurship)");}

    {auto& tc = list.Add("Aspirational/motivator");
    tc.Add("Tony Robbins (motivational speaking/coaching)");
    tc.Add("Les Brown (motivational speaking/motivation)");
    tc.Add("Eric Thomas (motivational speaking/sports)");
    tc.Add("Tai Lopez (entrepreneur/self-improvement)");
    tc.Add("Steve Jobs (technology/innovation)");}

    {auto& tc = list.Add("Personal/relatable");
    tc.Add("Humble The Poet (poetry/self-reflection)");
    tc.Add("Logan Paul (vlogging/lifestyle)");
    tc.Add("Tim Ferriss (self-experimentation/productivity)");
    tc.Add("James Altucher (self-improvement/entrepreneurship)");
    tc.Add("Jocko Willink (leadership/discipline)");}

    {auto& tc = list.Add("Inquisitive/thoughtful");
    tc.Add("Neil deGrasse Tyson (science/astrophysics)");
    tc.Add("Dan Harris (mindfulness/meditation)");
    tc.Add("Sam Harris (philosophy/science)");
    tc.Add("Tim Ferriss (self-experimentation/productivity)");
    tc.Add("Malcolm Gladwell (social science/writing)");}

    {auto& tc = list.Add("Educator/teacher");
    tc.Add("Sir Ken Robinson (education/creativity)");
    tc.Add("Russell Brunson (sales/marketing)");
    tc.Add("Simon Sinek (leadership/communication)");
    tc.Add("Stephen Covey (personal development/leadership)");
    tc.Add("Jordan Peterson (psychology/philosophy)");}
	
	ASSERT(GetPersonas().GetCount() == list.GetCount());
	return list;
}

VectorMap<String,Vector<String>>& GetPersonaUnsafeFemale() {
	thread_local static VectorMap<String,Vector<String>> list;
	ONCELOCK
	if (!list.IsEmpty()) return list;
	{auto& tc = list.Add("Expert/guru");
    tc.Add("Marie Forleo (entrepreneurship/coaching)");
    tc.Add("Rachel Hollis (personal development/wellness)");
    tc.Add("Jenna Kutcher (marketing/photography)");
    tc.Add("Gretchen Rubin (productivity/lifestyle)");
    tc.Add("Mel Robbins (motivational speaker/author)");}

    {auto& tc = list.Add("Aspiring/learner");
    tc.Add("Rachel Brathen (yoga/mental health)");
    tc.Add("Joy Cho (design/crafts)");
    tc.Add("Mimi Ikonn (fashion/beauty)");
    tc.Add("Erin Loechner (slow living/parenting)");
    tc.Add("Kelsey Miller (body positivity/feminism)");}

    {auto& tc = list.Add("Creative/innovative");
    tc.Add("Ingrid Nilsen (lifestyle/entertainment)");
    tc.Add("Aimee Song (fashion/interior design)");
    tc.Add("Justine Ezarik (technology/gaming)");
    tc.Add("Chrissy Teigen (humor/food)");
    tc.Add("Louise Pentland (diy/mom blogger)");}

    {auto& tc = list.Add("Influencer/trendsetter");
    tc.Add("Chiara Ferragni (fashion/beauty)");
    tc.Add("Huda Kattan (makeup/beauty)");
    tc.Add("Kristina Bazan (fashion/travel)");
    tc.Add("Ascia AKF (fashion/lifestyle)");
    tc.Add("Aimee Song (fashion/interior design)");}

    {auto& tc = list.Add("Relatable/real");
    tc.Add("Constance Hall (parenting/women's issues)");
    tc.Add("Grace Helbig (lifestyle/youtuber)");
    tc.Add("Hannah Hart (comedy/lifestyle)");
    tc.Add("Laura Lejeune (mental health/wellness)");
    tc.Add("Alisha Marie (lifestyle/diy)");}

    {auto& tc = list.Add("Hilarious/entertaining");
    tc.Add("Lilly Singh (comedy/entertainment)");
    tc.Add("Colleen Ballinger (comedy/entertainment)");
    tc.Add("GloZell Green (comedy/lifestyle)");
    tc.Add("Mamrie Hart (comedy/entertainment)");
    tc.Add("Jenny Lawson (humor/writing)");}

    {auto& tc = list.Add("Inspiring/motivator");
    tc.Add("Gabi Gregg (body positivity/fashion)");
    tc.Add("Elizabeth Gilbert (creativity/writing)");
    tc.Add("Tara Stiles (yoga/wellness)");
    tc.Add("Lalah Delia (self-care/spirituality)");
    tc.Add("Jen Sincero (personal development/money)");}

    {auto& tc = list.Add("Honest/truth-teller");
    tc.Add("Glennon Doyle (feminism/mental health)");
    tc.Add("Amy Schumer (comedy/feminism)");
    tc.Add("Michelle Obama (activism/women's issues)");
    tc.Add("Brené Brown (vulnerability/shame)");
    tc.Add("Esther Perel (relationships/sexuality)");}

    {auto& tc = list.Add("Authentic/transparent");
    tc.Add("Jen Hatmaker (faith/lifestyle)");
    tc.Add("Rachel Held Evans (faith/writing)");
    tc.Add("Leandra Medine (fashion/lifestyle)");
    tc.Add("Alexandria Ocasio-Cortez (politics/activism)");
    tc.Add("Glennon Doyle (feminism/mental health)");}

    {auto& tc = list.Add("Opinionated/thought-provoking");
    tc.Add("Anita Sarkeesian (media criticism/feminism)");
    tc.Add("Roxane Gay (writing/feminism)");
    tc.Add("Chimamanda Ngozi Adichie (literature/feminism)");
    tc.Add("Rupi Kaur (poetry/feminism)");
    tc.Add("Bree Newsome Bass (activism/politics)");}

    {auto& tc = list.Add("Down-to-earth/genuine");
    tc.Add("Candice Kumai (healthy living/recipe developer)");
    tc.Add("Constance Wu (acting/activism)");
    tc.Add("Chrissy Teigen (humor/motherhood)");
    tc.Add("Sophia Bush (activism/lifestyle)");
    tc.Add("Rachel Brathen (yoga/mental health)");}

    {auto& tc = list.Add("Sarcastic/witty");
    tc.Add("Samantha Irby (humor/lifestyle)");
    tc.Add("Mindy Kaling (comedy/writing)");
    tc.Add("Phoebe Robinson (comedy/pop culture)");
    tc.Add("Ali Wong (comedy/parenting)");
    tc.Add("Jenny Lawson (humor/writing)");}

    {auto& tc = list.Add("Chill/laid-back");
    tc.Add("Kate Hudson (fitness/wellness)");
    tc.Add("Agyness Deyn (fashion/activism)");
    tc.Add("Zoë Foster Blake (beauty/lifestyle)");
    tc.Add("Kristen Bell (acting/mental health)");
    tc.Add("Tess Holliday (body positivity/lifestyle)");}

    {auto& tc = list.Add("Sensible/practical");
    tc.Add("Joanna Gaines (design/home renovation)");
    tc.Add("Marie Forleo (entrepreneurship/business)");
    tc.Add("Grace Bonney (design/writing)");
    tc.Add("Kristina Bazan (fashion/entrepreneurship)");
    tc.Add("Sophia Amoruso (fashion/entrepreneurship)");}

    {auto& tc = list.Add("Spirited/energetic");
    tc.Add("Gabby Bernstein (self-help/guru)");
    tc.Add("Jen Sincero (self-help/writing)");
    tc.Add("Rachel Hollis (self-help/entrepreneurship)");
    tc.Add("Cat & Nat (parenting/entertainment)");
    tc.Add("Lilly Singh (comedy/entertainment)");}

    {auto& tc = list.Add("Curious/adventurous");
    tc.Add("Chrissy Teigen (food/travel)");
    tc.Add("The Bucket List Family (travel/adventure)");
    tc.Add("Gigi Gorgeous (beauty/identity)");
    tc.Add("Hannah Hart (food/travel/writing)");
    tc.Add("Cara Delevingne (fashion/identity)");}

    {auto& tc = list.Add("Analytical/problem-solver");
    tc.Add("Gretchen Rubin (self-help/writing)");
    tc.Add("Amy Porterfield (entrepreneurship/online marketing)");
    tc.Add("Tasha Eurich (psychology/self-discovery)");
    tc.Add("Jasmine Star (photography/marketing)");
    tc.Add("Susan Cain (psychology/self-improvement)");}

    {auto& tc = list.Add("Suppportive/encouraging");
    tc.Add("Brene Brown (self-help/writing)");
    tc.Add("Rachel Hollis (self-help/entrepreneurship)");
    tc.Add("Tara Mohr (self-improvement/career)");
    tc.Add("Gabby Bernstein (self-help/guru)");
    tc.Add("Mel Robbins (motivation/speaking)");}

    {auto& tc = list.Add("Bold/brave");
    tc.Add("Michelle Obama (politics/activism)");
    tc.Add("Lizzo (music/body positivity)");
    tc.Add("Lena Dunham (writing/activism)");
    tc.Add("Brené Brown (self-help/writing)");
    tc.Add("Malala Yousafzai (activism/education)");}

    {auto& tc = list.Add("Collaborative/connector");
    tc.Add("Jenna Kutcher (entrepreneurship/photography)");
    tc.Add("Bethany Mota (fashion/entrepreneurship)");
    tc.Add("Jess Lively (self-help/business)");
    tc.Add("Lauryn Evarts Bosstick (wellness/entrepreneurship)");
    tc.Add("Aimee Song (fashion/entrepreneurship)");}

    {auto& tc = list.Add("Aspirational/motivator");
    tc.Add("Rachel Hollis (self-help/entrepreneurship)");
    tc.Add("Marie Forleo (entrepreneurship/business)");
    tc.Add("Jen Sincero (self-help/writing)");
    tc.Add("Gretchen Rubin (self-help/writing)");
    tc.Add("Mel Robbins (motivation/speaking)");}

    {auto& tc = list.Add("Personal/relatable");
    tc.Add("Esther Perel (relationships/therapy)");
    tc.Add("Stephanie Buttermore (fitness/body positivity)");
    tc.Add("Brene Brown (self-help/writing)");
    tc.Add("Sali Hughes (beauty/writing)");
    tc.Add("Glennon Doyle (self-help/writing)");}

    {auto& tc = list.Add("Inquisitive/thoughtful");
    tc.Add("Elizabeth Gilbert (writing/life advice)");
    tc.Add("Sally Rooney (writing/philosophy)");
    tc.Add("Durga Chew-Bose (writing/personal essays)");
    tc.Add("Rebecca Solnit (writing/feminism)");
    tc.Add("Roxane Gay (writing/feminism)");}

    {auto& tc = list.Add("Educator/teacher");
    tc.Add("Rachel Cargle (activism/social justice)");
    tc.Add("Brené Brown (self-help/writing)");
    tc.Add("Marie Forleo (entrepreneurship/business)");
    tc.Add("Emily Weiss (beauty/entrepreneurship)");
    tc.Add("Mel Robbins (motivation/speaking)");}
    
	ASSERT(GetPersonas().GetCount() == list.GetCount());
	return list;
}

VectorMap<String,Vector<String>>& GetPersonaSafe(bool gender) {
	if (!gender)
		return GetPersonaSafeMale();
	else
		return GetPersonaSafeFemale();
}

VectorMap<String,Vector<String>>& GetPersonaUnsafe(bool gender) {
	if (!gender)
		return GetPersonaUnsafeMale();
	else
		return GetPersonaUnsafeFemale();
}

VectorMap<String,Vector<String>>& GetPersonaNiches(bool unsafe, bool gender) {
	if (!unsafe)
		return GetPersonaSafe(gender);
	else
		return GetPersonaUnsafe(gender);
}


END_TEXTLIB_NAMESPACE
