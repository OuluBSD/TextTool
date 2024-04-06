#include "ToolCore.h"
#include "ProtectedCommon.h"

BEGIN_TEXTLIB_NAMESPACE

const Index<String>& GetStyles() {
	static Index<String> list;
	ONCELOCK
	if (list.IsEmpty()) {
		list.Add("Intense");
		list.Add("Dark and brooding");
		list.Add("Whimsical and fantastical");
		list.Add("Sci-fi/Futuristic");
		list.Add("Film noir");
		list.Add("Comedic");
		list.Add("Romantic");
		list.Add("Family-friendly");
		list.Add("Adventure/Action");
		list.Add("Animated");
		list.Add("Period Piece");
		list.Add("Musical");
		list.Add("Dramatic");
		list.Add("Horror");
		list.Add("Mockumentary");
		list.Add("Documentary-style");
		list.Add("Experimental/Avant-garde");
		list.Add("Thriller/Suspense");
		list.Add("Mystery/Crime");
		list.Add("Western");
		list.Add("War/Epic");
		list.Add("Slice of Life");
		list.Add("Satire");
		list.Add("Surrealism");
	}
	return list;
}

const Vector<ContentType>& GetApproaches() {
	thread_local static Vector<ContentType> list;
	ONCELOCK
	if (list.IsEmpty()) {
		list.Add().Set("Sequential", "Linear", "Non-linear", "Cyclical");
		list.Add().Set("Parallel", "Simultaneous", "Interweaving", "Parallel Universes");
		list.Add().Set("Split-screen", "Side by Side", "Overlapping", "Contrasting");
		list.Add().Set("Montage", "Montage of Happiness", "Montage of Tragedy", "Anti-Montage");
		list.Add().Set("Flashback/Flashforward", "Nostalgic Flashback", "Traumatic Flashback", "Hopeful Flashforward");
		list.Add().Set("First person POV", "Immersive", "Introspective", "Detached");
		list.Add().Set("Multiple POVs", "Collage of Perspectives", "Unreliable Narrators", "Contrasting POVs");
		list.Add().Set("Non-linear", "Chaotic", "Fragmented", "Achronological");
		list.Add().Set("Linear", "Clear Narrative", "Progressive", "Predictable");
		list.Add().Set("Real-time", "Tension-building", "Intense", "Slow-paced");
		list.Add().Set("Dream sequence", "Surreal", "Nightmarish", "Fantastical");
		list.Add().Set("Voiceover narration", "Informative", "Reflective", "Misleading");
		list.Add().Set("Metaphorical/Symbolic", "Allegorical", "Metaphysical", "Literal");
		list.Add().Set("Documentary-style", "Realistic", "Mockumentary", "Biographical");
		list.Add().Set("Experimental/Avant-garde", "Non-conventional", "Deconstructed", "Provocative");
		list.Add().Set("Animated", "Cartoonish", "Gritty", "Anime-inspired");
		list.Add().Set("Mash-up", "Unexpected Combination", "Satirical", "Remixed");
		list.Add().Set("Remix", "Reimagined", "Modernized", "Reinvented");
		list.Add().Set("Virtual Reality/Augmented Reality", "Immersive", "Interactive", "Escapist");
		list.Add().Set("Collage", "Fragmented", "Mixed Media", "Chaos");
		list.Add().Set("Split-screen", "Parallel Stories", "Contrasting Perspectives", "Altered Reality");
		list.Add().Set("Flipbook", "Playful", "Rhythmic", "Nostalgic");
		list.Add().Set("Motion graphics", "Graphic", "Animated", "Minimalistic");
		list.Add().Set("Minimalist", "Simplistic", "Abstract", "Meditative");
		list.Add().Set("Surreal/Fantasy", "Dreamlike", "Whimsical", "Dark");
		list.Add().Set("Minimalist/Fluid storytelling", "Efficient", "Streamlined", "Dynamic");
	}
	return list;
}

int GetApproachCount() {
	return GetNiches().GetCount();
}


VectorMap<String,Vector<String>>& GetStyleSafeMale() {
	thread_local static VectorMap<String,Vector<String>> list;
	ONCELOCK
	if (!list.IsEmpty()) return list;
	
	{auto& tc = list.Add("Intense");
	tc.Add("The opening sequence of the movie Saving Private Ryan, with fast-paced action and intense battle scenes.");
	tc.Add("A suspenseful horror movie, with dark and moody visuals and heart-pumping music.");
	tc.Add("An action-packed spy thriller, with high-speed chase scenes and dramatic fight sequences.");
	tc.Add("A dramatic war epic, with intense battle scenes and emotional dialogue.");
	tc.Add("A crime drama, with gritty and intense visuals and dialogue.");
	tc.Add("A high-stakes sports movie, with intense training and competition scenes.");}

	{auto& tc = list.Add("Dark and brooding");
	tc.Add("A psychological thriller, with dark and mysterious visuals and dialogue.");
	tc.Add("A neo-noir crime drama, with moody and atmospheric shots.");
	tc.Add("A gothic vampire film, with dark and dramatic scenes.");
	tc.Add("A dystopian sci-fi movie, with a bleak and oppressive atmosphere.");
	tc.Add("A revenge thriller, with brooding and intense imagery.");
	tc.Add("A character study drama, with somber and introspective scenes.");}

	{auto& tc = list.Add("Whimsical and fantastical");
	tc.Add("A fantasy adventure, with vibrant and colorful visuals and fantastical creatures.");
	tc.Add("A family-friendly animated film, with playful and imaginative visuals.");
	tc.Add("A romantic comedy, with light-hearted and whimsical dialogue.");
	tc.Add("A coming-of-age drama, with fantastical and dreamlike sequences.");
	tc.Add("A musical set in a magical world, with enchanting and playful musical numbers.");
	tc.Add("A fairytale retelling, with charming and whimsical storytelling.");}

	{auto& tc = list.Add("Sci-fi/Futuristic");
	tc.Add("An epic space opera, with futuristic technology and otherworldly landscapes.");
	tc.Add("A mind-bending sci-fi thriller, with high-tech visuals and complex concepts.");
	tc.Add("A cyberpunk adventure, with sleek and futuristic visuals and action sequences.");
	tc.Add("A time-travel adventure, with futuristic and otherworldly landscapes.");
	tc.Add("A post-apocalyptic dystopia, with advanced technology and a bleak and desolate atmosphere.");
	tc.Add("A thought-provoking sci-fi drama, with futuristic concepts and emotional depth.");}

	{auto& tc = list.Add("Film noir");
	tc.Add("A neo-noir crime thriller, with shadowy and atmospheric visuals and complex characters.");
	tc.Add("A detective mystery, with moody and brooding shots and a hard-boiled protagonist.");
	tc.Add("A classic film noir, with stylish visuals and a suspenseful plot.");
	tc.Add("A dark and gritty detective drama, with a noir-inspired aesthetic.");
	tc.Add("A neo-noir sci-fi movie, with a futuristic twist on the classic film noir genre.");
	tc.Add("A psychological thriller with noir elements, featuring a morally ambiguous protagonist and a twisty plot.");}

	{auto& tc = list.Add("Comedic");
	tc.Add("A buddy comedy, with hilarious hijinks and banter between friends.");
	tc.Add("A slapstick comedy, with over-the-top physical comedy and goofy characters.");
	tc.Add("A romantic comedy, with funny and heartwarming moments between the couple.");
	tc.Add("A satire, with clever and witty commentary on societal issues or pop culture.");
	tc.Add("A workplace comedy, with quirky and relatable characters navigating office dynamics.");
	tc.Add("A parody of a specific genre or movie, with exaggerated and humorous takes on clichés.");}

	{auto& tc = list.Add("Romantic");
	tc.Add("A sweeping romance, with stunning visuals and passionate love scenes.");
	tc.Add("A romantic comedy, with both humorous and heartfelt moments between the couple.");
	tc.Add("A period piece romance, with elegant and romantic settings and costumes.");
	tc.Add("A tragic love story, with emotional and heart-wrenching scenes.");
	tc.Add("A fantasy romance, with a unique and romantic premise.");
	tc.Add("A romantic drama, with a realistic and emotionally-charged portrayal of a relationship.");}

	{auto& tc = list.Add("Family-friendly");
	tc.Add("A heartwarming family adventure, with lovable characters and feel-good moments.");
	tc.Add("An animated film for all ages, with vibrant and imaginative visuals and positive messages.");
	tc.Add("A coming-of-age story for kids, with relatable and empowering themes.");
	tc.Add("A fantasy adventure, with magical and enchanting worlds and characters.");
	tc.Add("A sports movie about teamwork and determination, with an inspirational message.");
	tc.Add("A wholesome family comedy, with lovable and quirky characters and funny situations.");}

	{auto& tc = list.Add("Adventure/Action");
	tc.Add("A high-stakes heist movie, with thrilling action sequences and a clever plot.");
	tc.Add("An epic adventure with daring stunts and exotic locations.");
	tc.Add("A spy thriller, with intense action and suspenseful spy missions.");
	tc.Add("A survival movie, with adrenaline-pumping scenes and a fight for survival.");
	tc.Add("An epic fantasy journey, with epic battles and fantastical creatures.");
	tc.Add("A post-apocalyptic adventure, with intense action and a quest for survival.");}

	{auto& tc = list.Add("Animated");
	tc.Add("An animated musical with catchy songs and lovable characters.");
	tc.Add("A comedy adventure with hilarious and endearing animated animals.");
	tc.Add("A coming-of-age story about a young hero on a quest, with stunning animation and a heartwarming message.");
	tc.Add("A computer-animated comedy starring a group of lovable and quirky characters.");
	tc.Add("A family-friendly animated film with a heartwarming message and visually stunning scenes.");
	tc.Add("An action-packed animated movie with a diverse and dynamic group of heroes.");}

	{auto& tc = list.Add("Period Piece");
	tc.Add("A historical drama set in a specific time period, with striking costumes and sets.");
	tc.Add("A romantic period piece, with sweeping romance and beautiful cinematography.");
	tc.Add("A biopic about a famous historical figure, with stunning visuals and a compelling story.");
	tc.Add("A war drama set in a specific era, with realistic and emotional depictions of historical events.");
	tc.Add("A costume drama, featuring lavish costumes and grand settings.");
	tc.Add("A literary adaptation set in the past, with rich storytelling and immersive period details.");}

	{auto& tc = list.Add("Musical");
	tc.Add("A Broadway musical adaptation with lavish sets and show-stopping musical numbers.");
	tc.Add("A jukebox musical featuring popular songs, with energetic dance numbers and a heartwarming story.");
	tc.Add("A musical biopic, with powerful performances and iconic songs.");
	tc.Add("A modern musical with catchy and original songs, featuring a diverse and talented cast.");
	tc.Add("A family-friendly movie with musical numbers and a heartwarming message.");
	tc.Add("A romantic musical with sweeping love songs and dazzling choreography.");}

	{auto& tc = list.Add("Dramatic");
	tc.Add("A character-driven drama, with intense and emotional scenes between complex characters.");
	tc.Add("A family drama with realistic and relatable portrayals of relationships and conflicts.");
	tc.Add("A legal drama, with gripping courtroom scenes and thought-provoking themes.");
	tc.Add("A biographical drama about a real-life figure, with powerful performances and a moving story.");
	tc.Add("An ensemble drama, with interconnected storylines and compelling character development.");
	tc.Add("A war drama, with powerful and emotional depictions of human struggle and sacrifice.");}

	{auto& tc = list.Add("Horror");
	tc.Add("A psychological horror movie, with mind-bending scares and a twisted plot.");
	tc.Add("A supernatural horror film, with eerie and unsettling visuals and jump scares.");
	tc.Add("A monster movie, with terrifying creatures and frightening encounters.");
	tc.Add("A slasher film, with suspenseful chase scenes and gruesome kills.");
	tc.Add("A horror comedy, with a mix of humor and scares.");
	tc.Add("A found footage-style movie, with a sense of realism and found footage elements.");}

	{auto& tc = list.Add("Mockumentary");
	tc.Add("A comedy mockumentary about a quirky workplace or community.");
	tc.Add("A satire/mockumentary about a specific topic or industry, poking fun at its tropes and clichés.");
	tc.Add("A found footage-style movie, parodying and subverting the genre's conventions.");
	tc.Add("A mockumentary about a fictional event or historical figure, with witty and humorous commentary.");
	tc.Add("A musical mockumentary, featuring mock interviews and musical numbers.");
	tc.Add("A slapstick comedy mockumentary, following the antics of a bumbling protagonist.");}

	{auto& tc = list.Add("Documentary-style");
	tc.Add("A historical documentary, presenting real events and interviews with expert commentary.");
	tc.Add("A nature documentary, with stunning footage and educational narration.");
	tc.Add("A true crime documentary, with gripping interviews and investigative storytelling.");
	tc.Add("A sports documentary, exploring the struggles and triumphs of athletes and teams.");
	tc.Add("A music documentary, exploring the careers and legacies of famous musicians.");
	tc.Add("A social issue/documentary hybrid, shining a light on important topics and featuring real people and their stories.");}

	{auto& tc = list.Add("Experimental/Avant-garde");
	tc.Add("A surreal and visually striking experimental film with no traditional narrative structure.");
	tc.Add("An avant-garde film exploring complex and abstract concepts through unique visual storytelling.");
	tc.Add("A non-linear experimental film, disorienting the viewer and challenging traditional storytelling conventions.");
	tc.Add("A hybrid of live-action and animation, blurring the lines between reality and fantasy.");
	tc.Add("A character study film with bold and unconventional techniques, diving deep into the psyche of the protagonist.");
	tc.Add("A highly stylized and visually stunning experimental film, exploring themes of identity and perception.");}

	{auto& tc = list.Add("Thriller/Suspense");
	tc.Add("A kidnapping thriller, with tense and suspenseful scenes and a race against time.");
	tc.Add("A psychological thriller, with mind games and a twisty plot.");
	tc.Add("A political thriller, with high-stakes and dangerous conspiracy theories.");
	tc.Add("A home invasion thriller, with a terrifying cat-and-mouse game between the villain and victim(s).");
	tc.Add("A survival thriller, with intense and suspenseful scenarios and a fight for survival.");
	tc.Add("A legal thriller, with courtroom drama and clever plot twists.");}

	{auto& tc = list.Add("Mystery/Crime");
	tc.Add("A murder mystery, with a complex and twisty plot and a mysterious killer.");
	tc.Add("A heist/caper movie, with clever schemes and intricate plans.");
	tc.Add("A detective/crime procedural, with a dedicated investigator and a riveting case.");
	tc.Add("A spy thriller, with a web of secrets and double-crossing characters.");
	tc.Add("A neo-noir crime drama, with gritty and atmospheric visuals and complex characters.");
	tc.Add("A psychological mystery, with a clever and unpredictable plot and mind-bending twists.");}

	{auto& tc = list.Add("Western");
	tc.Add("A classic western, with sweeping landscapes and a clash between good and evil.");
	tc.Add("A revisionist western, subverting traditional western tropes and featuring complex characters.");
	tc.Add("A western with a diverse and inclusive cast, breaking the mold of traditional cowboy stories.");
	tc.Add("A neo-western, with modern elements and a gritty and realistic portrayal of the old west.");
	tc.Add("A western drama, exploring themes of revenge, redemption, and justice.");
	tc.Add("A western adventure, with rugged cowboys and thrilling shootouts.");}

	{auto& tc = list.Add("War/Epic");
	tc.Add("An epic war drama, with stunning battle scenes and emotional depictions of the effects of war.");
	tc.Add("A military action movie, with intense and explosive battle sequences.");
	tc.Add("A war biopic, telling the incredible true story of a soldier or group of soldiers.");
	tc.Add("A historical epic set during a major war or conflict, with grand and epic battles.");
	tc.Add("A war adventure, with soldiers on a dangerous mission behind enemy lines.");
	tc.Add("A war romance, with a love story set against the backdrop of war and its challenges.");}

	{auto& tc = list.Add("Slice of Life");
	tc.Add("A coming-of-age story about a teenager navigating the ups and downs of high school.");
	tc.Add("A relatable comedy about the everyday struggles of a group of friends or coworkers.");
	tc.Add("A heartwarming drama about family dynamics and relationships.");
	tc.Add("A character study about an ordinary person facing challenges and growth in their life.");
	tc.Add("A dramedy about a person going through a major life change or crisis.");
	tc.Add("A slice-of-life mockumentary about the everyday struggles of a group of quirky characters.");}

	{auto& tc = list.Add("Satire");
	tc.Add("A political satire, poking fun at politicians and their policies.");
	tc.Add("A social satire, critiquing societal norms and expectations.");
	tc.Add("A dark comedy satire, exploring taboo topics and pushing the boundaries of humor.");
	tc.Add("A comedic satire about the entertainment industry or media.");
	tc.Add("A satire of a specific genre or style, parodying its tropes and clichés.");
	tc.Add("A satire of a specific historical event or figure, using humor to make a statement about its impact.");}

	{auto& tc = list.Add("Surrealism");
	tc.Add("A surreal and dreamlike film, blurring the lines between reality and imagination.");
	tc.Add("A satirical surreal comedy, using strange and unconventional elements to make a point.");
	tc.Add("A dark and unsettling surreal horror movie.");
	tc.Add("A surreal drama exploring themes of identity and existence.");
	tc.Add("A hybrid of animation and live-action, creating a whimsical and surreal world.");
	tc.Add("A mind-bending thriller with surreal and thought-provoking twists.");}
	
	ASSERT(GetStyles().GetCount() == list.GetCount());
	return list;
}

VectorMap<String,Vector<String>>& GetStyleSafeFemale() {
	thread_local static VectorMap<String,Vector<String>> list;
	ONCELOCK
	if (!list.IsEmpty()) return list;
    
	{auto& tc = list.Add("Intense");
	tc.Add("A thriller movie, with high-stakes and intense action sequences.");
	tc.Add("A drama, with emotionally charged scenes and intense relationships.");
	tc.Add("An adventure film, with heart-pumping action and intense survival scenarios.");
	tc.Add("A heist/crime movie, with tense planning and high-risk situations.");
	tc.Add("A revenge story, with powerful emotions and intense confrontations.");}

	{auto& tc = list.Add("Dark and brooding");
	tc.Add("A psychological thriller, with a dark and haunting atmosphere.");
	tc.Add("A mystery, with complex and brooding characters.");
	tc.Add("A survival story, set in a dark and foreboding environment.");
	tc.Add("A drama, exploring dark and difficult themes.");
	tc.Add("A fantasy or supernatural story, with a brooding and ominous tone.");}

	{auto& tc = list.Add("Whimsical and fantastical");
	tc.Add("A fairy tale adventure, with magical creatures and whimsical landscapes.");
	tc.Add("A rom-com, with lighthearted humor and dreamy romance.");
	tc.Add("A musical, with vibrant and fantastical musical numbers.");
	tc.Add("An animated movie, with charming characters and a whimsical storyline.");
	tc.Add("A fantasy epic, with imaginative worlds and fantastical creatures.");}

	{auto& tc = list.Add("Sci-fi/Futuristic");
	tc.Add("A dystopian world, with advanced technology and a struggle for survival.");
	tc.Add("A space adventure, with futuristic spacecrafts and otherworldly creatures.");
	tc.Add("A futuristic society, exploring themes of technology and its impact on humanity.");
	tc.Add("A time-travel story, with futuristic gadgets and complex timelines.");
	tc.Add("A post-apocalyptic setting, with advanced technology used for survival.");}

	{auto& tc = list.Add("Film noir");
	tc.Add("A crime/mystery movie, with a dark and gritty portrayal of the seedy underbelly of society.");
	tc.Add("A drama, exploring themes of guilt, corruption, and moral ambiguity.");
	tc.Add("A thriller, with a bleak and moody atmosphere and suspenseful plot twists.");
	tc.Add("A period piece, set in the 1940s-1950s with classic film noir elements such as femme fatales and hard-boiled detectives.");
	tc.Add("A neo-noir film, modernizing the classic style with modern settings and characters.");}

	{auto& tc = list.Add("Comedic");
	tc.Add("A romantic comedy, with witty banter and hilarious situations.");
	tc.Add("A comedy of errors, with mistaken identities and outrageous mishaps.");
	tc.Add("An ensemble comedy, with a diverse group of characters and humorous interactions.");
	tc.Add("A parody or spoof, poking fun at popular movies or genres.");
	tc.Add("A coming-of-age comedy, with relatable and awkward teenage experiences.");}

	{auto& tc = list.Add("Romantic");
	tc.Add("A romantic drama, with heartwarming and emotional love stories.");
	tc.Add("A rom-com, with charming and humorous love interests and romantic escapades.");
	tc.Add("A historical romance, set in a specific time period with a passionate and sweeping love story.");
	tc.Add("A fantasy romance, with a mythical or supernatural element adding to the love story.");
	tc.Add("A romantic tragedy, with forbidden love and heart-wrenching sacrifices.");}

	{auto& tc = list.Add("Family-friendly");
	tc.Add("An animated adventure, suitable for all ages with positive messages and lovable characters.");
	tc.Add("A heartwarming family drama, exploring themes of love, forgiveness, and coming together.");
	tc.Add("A fantasy movie, suitable for the whole family with fantastical creatures and magical adventures.");
	tc.Add("A musical, with catchy songs and an uplifting story that appeals to all ages.");
	tc.Add("A comedy, with clean humor and relatable family dynamics.");}

	{auto& tc = list.Add("Adventure/Action");
	tc.Add("An epic adventure, with thrilling action sequences and a quest for treasure or a mythical artifact.");
	tc.Add("An action-thriller, with high-speed chases and intense fighting scenes.");
	tc.Add("A survival story, with characters pushed to their limits in extreme and dangerous conditions.");
	tc.Add("A period piece action movie, set in a specific time period and incorporating historical events into the action-packed plot.");
	tc.Add("A superhero movie, with a female protagonist and her heroic journey to save the world.");}

	{auto& tc = list.Add("Animated");
	tc.Add("A musical, with vibrant and colorful animation and catchy songs.");
	tc.Add("An adventure movie, with lovable animated characters and a grand journey.");
	tc.Add("A fantasy story, with imaginative and fantastical animation.");
	tc.Add("A comedy, with silly and zany animated characters and humorous situations.");
	tc.Add("A family-friendly movie, with heartwarming and relatable animated characters and positive messages.");}

	{auto& tc = list.Add("Period Piece");
	tc.Add("A historical drama, exploring a specific time period with realistic and accurate depictions.");
	tc.Add("A romance, set in a specific time period with alluring costumes and settings.");
	tc.Add("A biographical film, relaying the story of a real woman in history and her impact on the world.");
	tc.Add("A war movie, set in a particular historical era and showcasing the experiences of women during wartime.");
	tc.Add("A fantasy or fairy tale, incorporating elements of a specific time period and reimagining them through a female perspective.");}

	{auto& tc = list.Add("Musical");
	tc.Add("A romantic musical, with love ballads and grand musical numbers.");
	tc.Add("A drama, with emotional and powerful songs that drive the story forward.");
	tc.Add("A jukebox musical, incorporating popular songs into the storyline and showcasing the talent of the female lead.");
	tc.Add("A fantasy or animated musical, using music to enhance the magical or fantastical elements of the story.");
	tc.Add("A Broadway-inspired musical, with elaborate choreography and show-stopping performances.");}

	{auto& tc = list.Add("Dramatic");
	tc.Add("A character-driven drama, delving into the complex emotions and relationships of the main character.");
	tc.Add("A biographical film, showcasing the struggles and triumphs of a real woman in history.");
	tc.Add("A period piece, portraying the societal and personal challenges faced by women in a specific time period.");
	tc.Add("A political drama, tackling relevant political issues and how they impact women's lives.");
	tc.Add("A courtroom drama, following a female lawyer's journey to justice and navigating a male-dominated legal system.");}

	{auto& tc = list.Add("Horror");
	tc.Add("A supernatural horror, with ghostly apparitions and sinister spirits.");
	tc.Add("A psychological horror, delving into the dark and disturbed minds of the characters.");
	tc.Add("A survival horror, with a female lead fighting to survive against terrifying threats.");
	tc.Add("A slasher movie, with fast-paced action and thrilling suspense.");
	tc.Add("A horror-comedy, blending humor with horror for a fun and spooky viewing experience.");}

	{auto& tc = list.Add("Mockumentary");
	tc.Add("A mockumentary-style comedy, with a playful and satirical take on a specific topic or genre.");
	tc.Add("A mockumentary inspired by real-life events, exploring the absurdity and humor in everyday situations.");
	tc.Add("A political mockumentary, satirizing current events and societal issues through a female perspective.");
	tc.Add("A horror mockumentary, using a faux documentary style to add a sense of realism to the scary elements of the story.");
	tc.Add("A mockumentary following the behind-the-scenes antics of a group of female filmmakers, touching on the challenges and successes of being a woman in the industry.");}

	{auto& tc = list.Add("Documentary-style");
	tc.Add("A biographical documentary, showcasing the life and achievements of a real woman in history.");
	tc.Add("A feminist documentary, exploring important issues facing women today and highlighting the voices and experiences of women from diverse backgrounds.");
	tc.Add("A nature or wildlife documentary, showcasing the beauty and power of the natural world and its impact on women's lives.");
	tc.Add("A sports documentary, highlighting the accomplishments and challenges faced by female athletes.");
	tc.Add("A cultural documentary, shedding light on the unique experiences and perspectives of women from different cultures and backgrounds.");}

	{auto& tc = list.Add("Experimental/Avant-garde");
	tc.Add("An abstract art film, using unique and unconventional techniques to explore themes of femininity and identity.");
	tc.Add("An experimental narrative, pushing the boundaries of traditional storytelling with surreal and thought-provoking visuals.");
	tc.Add("An avant-garde film, showcasing unconventional and provocative themes and challenging societal norms and expectations.");
	tc.Add("A performance art piece, combining different mediums such as music, dance, and visuals to tell a thought-provoking story.");
	tc.Add("A virtual reality experience, immersing viewers in a visually stunning and thought-provoking journey through the female perspective.");}

	{auto& tc = list.Add("Thriller/Suspense");
	tc.Add("A psychological thriller, with mind-bending twists and turns that keep viewers on the edge of their seats.");
	tc.Add("A crime thriller, with high-stakes and intense investigations led by a female detective.");
	tc.Add("A domestic thriller, exploring the dark secrets and dangers hiding within seemingly perfect families.");
	tc.Add("A conspiracy thriller, following a female journalist's pursuit of the truth behind a dangerous cover-up.");
	tc.Add("A political thriller, tackling relevant and timely issues and the potentially deadly consequences faced by women trying to uncover the truth.");}

	{auto& tc = list.Add("Mystery/Crime");
	tc.Add("A detective movie, following a female investigator solving a challenging and complex case.");
	tc.Add("A whodunit mystery, with an intriguing and suspenseful plot that keeps viewers guessing until the very end.");
	tc.Add("A crime drama, delving into the lives and motivations of criminals and the women who are determined to take them down.");
	tc.Add("A noir-inspired mystery, filled with intrigue and deception in a dark and gritty setting.");
	tc.Add("A cozy mystery, featuring a strong and relatable female lead solving crimes in a charming small town.");}

	{auto& tc = list.Add("Western");
	tc.Add("A classic Western, with a strong and resilient female protagonist navigating the challenges of the Wild West.");
	tc.Add("A modern Western, exploring contemporary themes and social issues while still maintaining the spirit of the genre.");
	tc.Add("A Western with a feminist twist, highlighting the often overlooked roles and contributions of women in the Wild West.");
	tc.Add("A female-led revenge story, set in the harsh and unforgiving landscape of the Wild West.");
	tc.Add("A Western adventure, following a group of women on a daring journey through the rugged frontier.");}

	{auto& tc = list.Add("War/Epic");
	tc.Add("A historical war movie, portraying the experiences of female soldiers and civilians during a specific war or conflict.");
	tc.Add("A fantasy epic, with strong and capable female warriors fighting for survival in a treacherous and magical world.");
	tc.Add("A modern war movie, shedding light on the physical and emotional toll of war on women.");
	tc.Add("An epic journey, following a group of women on a perilous and heroic quest to save their homeland or their loved ones.");
	tc.Add("A war romance, showcasing the resilience and strength of love amidst the chaos and destruction of war.");}

	{auto& tc = list.Add("Slice of Life");
	tc.Add("A drama, delving into the everyday struggles and triumphs of a relatable female lead.");
	tc.Add("A romantic comedy, following the awkward and endearing moments of a woman navigating modern dating.");
	tc.Add("A coming-of-age story, exploring the challenges and joys of growing up as a young woman in today's society.");
	tc.Add("A family drama, touching on the complexities and dynamics of familial relationships.");
	tc.Add("A workplace comedy or drama, focusing on the experiences of women in a specific industry or job.");}

	{auto& tc = list.Add("Satire' :");
	tc.Add("A political satire, satirizing the absurdities and hypocrisies of gender roles and societal expectations.");
	tc.Add("A parody of a popular film or TV show, providing a humorous and witty commentary on gender stereotypes and tropes.");
	tc.Add("A comedy sketch or series, cleverly mocking the portrayal of women in media and entertainment.");
	tc.Add("A mockumentary, lampooning the stereotypes and misconceptions surrounding women in a certain field or industry.");
	tc.Add("A satirical examination of modern relationships and dating culture.");}

	{auto& tc = list.Add("Surrealism");
	tc.Add("An experimental short film, challenging traditional storytelling and exploring the complexities of female identity and experience.");
	tc.Add("A surreal fantasy, blurring the lines between dreams and reality and showcasing the inner world of a female protagonist.");
	tc.Add("A mind-bending thriller, following a woman on a journey through her own subconscious as she grapples with her desires and fears.");
	tc.Add("An avant-garde exploration of femininity and womanhood, using abstract imagery and symbolism.");
	tc.Add("A magical realism tale, uncovering the hidden struggles and triumphs of everyday women through fantastical elements. ");}

	ASSERT(GetStyles().GetCount() == list.GetCount());
	return list;
}

VectorMap<String,Vector<String>>& GetStyleUnsafeMale() {
	thread_local static VectorMap<String,Vector<String>> list;
	ONCELOCK
	if (!list.IsEmpty()) return list;
	
	{auto& tc = list.Add("Intense");
	tc.Add("A gritty and violent action-thriller, featuring a male protagonist on a mission for revenge.");
	tc.Add("A psychological drama, delving into the dark and intense inner thoughts and struggles of a male character.");
	tc.Add("A graphic crime thriller, showcasing brutal and intense scenes of violence.");
	tc.Add("A high-stakes heist story, filled with intense action and suspense as a group of male criminals attempt to pull off a daring robbery.");
	tc.Add("An intense and explicit drama, exploring the complexities of male relationships and the consequences of their actions. ");}

	{auto& tc = list.Add("Dark and brooding");
	tc.Add("A gritty and gritty noir-style detective story, following a male detective as he navigates the corrupt and dangerous underbelly of the city.");
	tc.Add("A psychological horror/thriller, delving into the troubled mind of a male character haunted by nightmares and delusions.");
	tc.Add("A supernatural thriller, centered around a male protagonist who discovers he has dark and dangerous powers.");
	tc.Add("A post-apocalyptic survival story, focused on a male character struggling to survive in a world overrun by violence and despair.");
	tc.Add("A disturbing and twisted portrayal of toxic masculinity, exploring the destructive consequences of male power and privilege.");}

	{auto& tc = list.Add("Whimsical and fantastical");
	tc.Add("A dark and twisted fairy tale, featuring a male hero on a magical quest to save a cursed princess.");
	tc.Add("A dark comedy following the misadventures of a group of male friends who accidentally unleash a powerful and mischievous spirit.");
	tc.Add("A surreal and trippy fantasy adventure, following a male protagonist as he journeys through a fantastical and bizarre world.");
	tc.Add("An R-rated parody of classic fantasy tropes, featuring a male character who must save the kingdom from an evil villain, with plenty of raunchy jokes along the way.");
	tc.Add("A twisted and dark retelling of a classic fairy tale, with unexpected twists and turns and a male anti-hero at the center.");}

	{auto& tc = list.Add("Sci-fi/Futuristic");
	tc.Add("A futuristic action-adventure, following a male hero as he fights against a tyrannical government in a world ruled by technology.");
	tc.Add("A mind-bending sci-fi thriller, following a male character who must unravel the truth about his own existence and the world he lives in.");
	tc.Add("A dark and gritty cyberpunk story, delving into the seedy underworld of a futuristic city and the male characters who inhabit it.");
	tc.Add("A sci-fi horror, featuring a male astronaut who must confront unimaginable horrors on a distant planet.");
	tc.Add("A sexy and provocative sci-fi drama, exploring themes of love, loss, and identity as two male characters find themselves in a world of advanced technology and complicated relationships. ");}

	{auto& tc = list.Add("Film noir");
	tc.Add("A classic neo-noir mystery, featuring a male private detective drawn into a web of deception and murder.");
	tc.Add("A gritty and atmospheric crime drama, following a group of male criminals as they plan and execute a dangerous heist.");
	tc.Add("A hard-boiled detective story, featuring a male detective navigating the corrupt and violent world of organized crime.");
	tc.Add("A dark and twisted tale of betrayal and vengeance, centered around a male protagonist seeking revenge for the death of his lover.");
	tc.Add("A neo-noir anthology following various male characters and their descent into darkness, each story connected by a common thread. ");}

	{auto& tc = list.Add("Comedic");
	tc.Add("A raunchy and irreverent comedy, following the misadventures of a group of male friends as they navigate love, life, and friendship.");
	tc.Add("A dark comedy about a male character who accidentally sells his soul to the devil and must find a way to get it back.");
	tc.Add("An outrageous and outrageous road trip comedy, featuring a group of male buddies on a wild and crazy journey.");
	tc.Add("A satire on toxic masculinity, poking fun at male stereotypes and societal expectations.");
	tc.Add("A black comedy about a dysfunctional family of male characters, featuring plenty of dark humor and twisted scenarios. ");}

	{auto& tc = list.Add("Romantic");
	tc.Add("A steamy and passionate romance, following the tumultuous love affair between two male characters.");
	tc.Add("A dramatic love triangle between three male characters, exploring themes of love, jealousy, and desire.");
	tc.Add("A forbidden romance between two male characters from rival gangs or families, facing challenges and dangers in their pursuit of love.");
	tc.Add("A coming-of-age story about a young male character discovering his sexuality and falling in love for the first time.");
	tc.Add("A sensual and erotic love story between two male characters, exploring the complexities of their relationship and the societal pressures they face. ");}

	{auto& tc = list.Add("Family-friendly");
	tc.Add("A heartwarming adventure story, featuring a male protagonist on a journey to reconnect with his estranged family.");
	tc.Add("A light-hearted comedy about a single father and his relationship with his young son.");
	tc.Add("An animated adventure about a group of male friends on a quest to save their hometown from an evil villain.");
	tc.Add("A heartwarming drama about a male character and his unconditional love for his adopted child.");
	tc.Add("A feel-good sports story about a male athlete and his close relationship with his father as they navigate the challenges of the game.");}

	{auto& tc = list.Add("Adventure/Action");
	tc.Add("An epic and adrenaline-fueled action movie, featuring a male hero on a mission to save the world.");
	tc.Add("A gritty and intense survival story, following a male character stranded on a remote island or in a hostile environment.");
	tc.Add("A high-stakes adventure, filled with daring stunts and dangerous obstacles as a group of male characters journey through uncharted territories.");
	tc.Add("A military action thriller, following a group of male soldiers on a dangerous mission in a war-torn country.");
	tc.Add("A fast-paced and exciting treasure hunt, featuring a male character and his team searching for a valuable and elusive artifact. ");}

	{auto& tc = list.Add("Animated");
	tc.Add("A dark and twisted animated film, following the journey of a male protagonist through a fantastical and surreal world.");
	tc.Add("A raunchy and irreverent adult animated comedy, featuring male characters and their outrageous antics.");
	tc.Add("An animated anthology series, featuring various male characters and their unique and bizarre stories.");
	tc.Add("A dark and provocative anime-style film, exploring themes of violence, sexuality, and identity through its male characters.");
	tc.Add("A coming-of-age animated movie about a young male character exploring his sexuality and relationships. ");}

	{auto& tc = list.Add("Period Piece");
	tc.Add("A lush and romantic drama, set in a past era and following the passionate and tumultuous relationships of male characters.");
	tc.Add("An epic historical adventure, featuring male characters in the midst of a war or uprising.");
	tc.Add("A biographical film about a male historical figure, delving into their personal struggles and accomplishments.");
	tc.Add("A dark and twisted retelling of a classic literary work, featuring male characters in a period setting.");
	tc.Add("A provocative and sensual exploration of the lives and desires of male characters in a past era. ");}

	{auto& tc = list.Add("Musical");
	tc.Add("A raunchy and irreverent musical comedy, featuring male characters and their outrageous musical numbers.");
	tc.Add("A dramatic and emotional journey through the music industry, following a male musician's rise to fame and the challenges they face.");
	tc.Add("A romantic musical about two male characters finding love and expressing their emotions through song and dance.");
	tc.Add("An animated musical about a group of male friends forming a band and striving for success.");
	tc.Add("A provocative and sensual musical exploring themes of love, lust, and betrayal through its male characters.");}

	{auto& tc = list.Add("Dramatic");
	tc.Add("A gripping and emotional drama, featuring a male protagonist facing personal challenges and obstacles.");
	tc.Add("A character study of a male character dealing with addiction, mental illness, or trauma.");
	tc.Add("A tense and intense legal drama, following a male lawyer fighting for justice and facing moral dilemmas.");
	tc.Add("A family drama about a male character struggling to balance his career and family life.");
	tc.Add("A dark and twisted exploration of the complexities and secrets of male relationships. ");}

	{auto& tc = list.Add("Horror");
	tc.Add("A chilling and atmospheric horror film, featuring a male character being stalked by a sinister presence.");
	tc.Add("A supernatural horror centered around a male character who uncovers a dark and dangerous secret about his family.");
	tc.Add("A psychological horror, following a male character as he becomes increasingly paranoid and delusional.");
	tc.Add("A slasher film with a male killer and his victims, featuring intense and graphic violence.");
	tc.Add("A horror anthology featuring various male characters encountering terrifying and otherworldly beings. ");}

	{auto& tc = list.Add("Mockumentary");
	tc.Add("A satirical mockumentary about the absurdities of male-dominated industries or societies.");
	tc.Add("A dark and twisted mockumentary about a male celebrity's fall from grace.");
	tc.Add("A hilarious and irreverent mockumentary following a group of male friends vying for a record deal in the music industry.");
	tc.Add("A dark comedy mockumentary about a group of male actors navigating the cutthroat world of Hollywood.");
	tc.Add("A controversial mockumentary delving into the private lives and scandals of male politicians or public figures.");}

	{auto& tc = list.Add("Documentary-style");
	tc.Add("A gritty and intense documentary-style film exploring the dangerous and illegal activities of male criminals.");
	tc.Add("A biographical documentary about a male historical figure, delving into their personal struggles and accomplishments.");
	tc.Add("A dark and provocative investigation into the world of male strip clubs and the lives of the dancers.");
	tc.Add("A controversial and thought-provoking documentary about the toxic effects of masculinity on society.");
	tc.Add("An exploration of the male psyche through the eyes of various real-life male subjects. ");}

	{auto& tc = list.Add("Experimental/Avant-garde");
	tc.Add("A surreal and visually stunning experiment in storytelling, featuring a male protagonist on a journey of self-discovery.");
	tc.Add("A thought-provoking and controversial avant-garde exploration of toxic masculinity and societal expectations of male behavior.");
	tc.Add("An abstract and dreamlike portrait of male identity and sexuality.");
	tc.Add("A dark and twisted avant-garde film about a male character's descent into madness.");
	tc.Add("A mind-bending and unconventional film about the blurred lines between reality and fantasy, featuring male characters at the center. ");}

	{auto& tc = list.Add("Thriller/Suspense");
	tc.Add("A gripping and intense suspense thriller, featuring a male protagonist trapped in a dangerous and terrifying situation.");
	tc.Add("A twisty and unpredictable psychological thriller, following a male character as he becomes a suspect in a murder investigation.");
	tc.Add("A cat-and-mouse game between a male detective and a cunning and dangerous criminal.");
	tc.Add("A political thriller, featuring male politicians and their manipulative games for power.");
	tc.Add("A dark and suspenseful thriller about a male hostage trying to escape the clutches of his captors. ");}

	{auto& tc = list.Add("Mystery/Crime");
	tc.Add("A twisty and suspenseful mystery, featuring a male detective trying to solve a complex and perplexing case.");
	tc.Add("A gritty and graphic crime drama, exploring the violent and corrupt world of organized crime through its male characters.");
	tc.Add("A chilling and disturbing mystery about a male character who believes he is being haunted by a ghost.");
	tc.Add("A noir-style murder mystery, following a male private investigator as he unravels a web of deceit and betrayal.");
	tc.Add("A tense and atmospheric mystery about a group of male friends on a camping trip, facing a series of deadly and mysterious events. ");}

	{auto& tc = list.Add("Western");
	tc.Add("An epic and action-packed western, featuring a male gunslinger on a quest for revenge.");
	tc.Add("A gritty and realistic depiction of the Wild West and the trials and tribulations of a group of male cowboys.");
	tc.Add("A revisionist western, exploring themes of masculinity and violence in the American West through its male characters.");
	tc.Add("A dark and brooding western, following a group of male outlaws on the run from the law.");
	tc.Add("A character-driven western about a male rancher and his struggles to protect his land and family from outside forces. ");}

	{auto& tc = list.Add("War/Epic");
	tc.Add("An epic war movie, following a group of male soldiers in the midst of a brutal and harrowing battle.");
	tc.Add("A gritty and realistic portrayal of the effects of war on male soldiers, both physically and mentally.");
	tc.Add("A political drama set during a war, featuring male generals and politicians making strategic decisions and facing moral dilemmas.");
	tc.Add("A dark and violent war story about a group of male soldiers on a dangerous mission behind enemy lines");
	tc.Add("An epic story of brotherhood and survival set during a war, following a group of male soldiers as they fight to make it home alive.");}

	{auto& tc = list.Add("Slice of Life");
	tc.Add("A raw and honest portrayal of the struggles and triumphs of a group of male friends living in a big city.");
	tc.Add("A coming-of-age story about a young male trying to navigate his relationships, career, and personal growth.");
	tc.Add("An intimate and sensual story about a male character discovering his own sexuality and desires.");
	tc.Add("A character-driven drama about a male character dealing with the aftermath of a life-changing event.");
	tc.Add("A gritty and realistic look at the daily struggles of blue-collar male workers in a small town.");}

	{auto& tc = list.Add("Satire");
	tc.Add("A satirical comedy about a group of male celebrities and their over-the-top antics and lifestyles.");
	tc.Add("A political satire about a male politician and the ridiculous and corrupt world of politics.");
	tc.Add("A darkly comedic exploration of toxic masculinity and the pressure of being a \"perfect\" man.");
	tc.Add("A satirical take on traditional fairy tales, featuring a male protagonist navigating a world of fairy tale characters and absurd situations.");
	tc.Add("An edgy and irreverent satire about a male writer trying to make it in the cutthroat world of literature.");}

	{auto& tc = list.Add("Surrealism");
	tc.Add("A trippy and mind-bending exploration of the subconscious through the eyes of a male protagonist.");
	tc.Add("A surreal and psychedelic journey through a male character's dream world.");
	tc.Add("An avant-garde and experimental film exploring themes of identity and masculinity through a male character's surreal experiences.");
	tc.Add("A dark and twisted horror story about a male character trapped in a bizarre and surreal alternate reality.");
	tc.Add("A surreal and absurdist comedy following a group of male roommates living in a strange and surreal apartment building.");}

	ASSERT(GetStyles().GetCount() == list.GetCount());
	return list;
}

VectorMap<String,Vector<String>>& GetStyleUnsafeFemale() {
	thread_local static VectorMap<String,Vector<String>> list;
	ONCELOCK
	if (!list.IsEmpty()) return list;
	
	{auto& tc = list.Add("Intense");
	tc.Add("A psychological thriller about a female protagonist who becomes obsessed with her mysterious and enigmatic neighbor.");
	tc.Add("A violent and shocking revenge story, following a fierce and determined female character seeking vengeance on those who have wronged her.");
	tc.Add("A female-led survival horror, following a group of women stranded on a remote island and fighting for their lives against a deadly creature.");
	tc.Add("A provocative and intense drama about a female politician navigating a corrupt and cutthroat world.");
	tc.Add("A gritty and adrenaline-fueled action flick, showcasing a skilled and resourceful female protagonist on a mission to take down a powerful drug lord.");}

	{auto& tc = list.Add("Dark and brooding");
	tc.Add("A Gothic romance filled with forbidden love, betrayal, and supernatural elements.");
	tc.Add("A horror tale about a group of female friends who stumble upon a cursed object, leading to terrifying and macabre consequences.");
	tc.Add("A noir-style crime drama, following a femme fatale who uses her charms and cunning to manipulate and outsmart the men in her life.");
	tc.Add("A slow-burning drama about a woman slowly unraveling as she becomes consumed by grief and loss.");
	tc.Add("A dark and twisted fantasy about a group of women with magical abilities who are caught in a power struggle with malevolent forces.");}

	{auto& tc = list.Add("Whimsical and fantastical");
	tc.Add("A whimsical fairytale about a young princess who must rescue her kingdom from an evil sorcerer with the help of her magical animal companions.");
	tc.Add("A magical coming-of-age story about a teenage girl who discovers she is a powerful witch and must learn to control her abilities while navigating the challenges of high school.");
	tc.Add("A colorful and delightful adventure about a group of female misfits on a quest to find a legendary treasure and save their village from destruction.");
	tc.Add("A quirky and surreal comedy about a woman who inherits a talking rabbit and must use its mystical powers to defeat an evil wizard.");
	tc.Add("A heartwarming fantasy about a girl who is transported to a land of mythical creatures and must go on a quest to find her way home.");}

	{auto& tc = list.Add("Sci-fi/Futuristic");
	tc.Add("An epic space opera following a badass female captain and her diverse and capable crew as they embark on dangerous missions across the galaxy.");
	tc.Add("A dystopian thriller about a group of women who must band together to overthrow a tyrannical government that seeks to control and oppress them.");
	tc.Add("A cyberpunk adventure following a skilled hacker and her team as they navigate a futuristic world filled with advanced technology and corrupt corporations.");
	tc.Add("A mind-bending sci-fi drama about a woman who discovers she has the ability to time travel, but must face the consequences of changing the past.");
	tc.Add("A post-apocalyptic survival story about a group of women who must rebuild society in a world devastated by a catastrophic event.");}

	{auto& tc = list.Add("Film noir");
	tc.Add("A sultry and seductive crime drama about a detective on the brink of retirement who gets caught up in a dangerous affair with a mysterious woman involved in a high-stakes heist.");
	tc.Add("A gritty and suspenseful thriller about a female journalist investigating a string of murders and facing off against a corrupt and powerful figure.");
	tc.Add("A stylish and twisted neo-noir about a femme fatale who manipulates her way into the inner circle of a wealthy and powerful family.");
	tc.Add("A moody and atmospheric detective story about a female private eye who must navigate the treacherous world of organized crime and corrupt officials.");
	tc.Add("A psychological thriller about a woman who becomes entangled in a dangerous love triangle with two men who may not be who they seem.");}

	{auto& tc = list.Add("Comedic");
	tc.Add("A raunchy and outrageous comedy about a group of girlfriends who go on a wild bachelorette weekend and end up getting into all sorts of hilarious shenanigans.");
	tc.Add("A buddy comedy about two women, one a no-nonsense detective and the other a quirky psychic, who team up to solve a case and become unlikely friends in the process.");
	tc.Add("A satirical workplace comedy about a group of ambitious and cutthroat women trying to climb the corporate ladder at a male-dominated company.");
	tc.Add("A silly and silly road trip comedy about two estranged sisters who must travel together to spread their grandmother's ashes and reconnect along the way.");
	tc.Add("A romantic comedy about two best friends who make a pact to get married if they are both still single by a certain age, and then unexpectedly find themselves falling for each other.");}

	{auto& tc = list.Add("Romantic");
	tc.Add("A sweeping historical romance about forbidden love between a noblewoman and a commoner in a time of war and political intrigue.");
	tc.Add("A modern day rom-com about a successful career woman who hires a male escort to pose as her boyfriend for a family wedding, only to find real feelings developing.");
	tc.Add("A steamy and passionate love story between two women who must navigate societal expectations and family disapproval in order to be together.");
	tc.Add("A romantic drama about a woman who falls in love with a mysterious and troubled stranger, only to discover he has a dangerous secret that threatens to tear them apart.");
	tc.Add("A charming and heartwarming tale about a woman who inherits a rundown cottage and hires a handsome carpenter to help repair it, leading to sparks and unexpected romance.");}

	{auto& tc = list.Add("Family-friendly");
	tc.Add("A heartwarming animated adventure about a young girl and her animal friends going on a quest to save their magical kingdom from an evil sorcerer.");
	tc.Add("A playful and wholesome comedy about a group of mothers who form a support group and go on fun and wacky outings together.");
	tc.Add("A heartwarming drama about a single mother trying to make a fresh start with her rebellious teenage daughter in a small town.");
	tc.Add("A fantasy adventure about a family of witches on a mission to stop an ancient curse from destroying their town and their magical way of life.");
	tc.Add("A heartwarming story about a girl who befriends a lonely mermaid and must help her return to her underwater kingdom before it's too late.");}

	{auto& tc = list.Add("Adventure/Action");
	tc.Add("A high-stakes spy thriller about a skilled and fearless female agent on a mission to stop a terrorist plot.");
	tc.Add("A swashbuckling pirate adventure about a clever and resourceful female captain and her crew as they navigate treacherous waters and rival pirates.");
	tc.Add("A survival drama about a group of women stranded in the wilderness after a plane crash and struggling to find a way back to civilization.");
	tc.Add("A post-apocalyptic action story about a tough and determined woman leading a band of survivors against ruthless and dangerous enemies.");
	tc.Add("A thrilling heist film about a group of female thieves planning and executing an elaborate and daring robbery.");}

	{auto& tc = list.Add("Animated");
	tc.Add("A magical and heartwarming tale about a girl who discovers she has the power to bring her drawings to life and embarks on a journey to save her family and friends from a dark force.");
	tc.Add("A sweeping fantasy epic about a young princess and her animal companions fighting against a tyrannical ruler to reclaim her rightful place as queen.");
	tc.Add("A musical adventure about a girl and her talking animal sidekick as they embark on a journey to find a legendary treasure and save their kingdom.");
	tc.Add("A high-energy and hilarious comedy about a group of female cartoon characters trying to escape their creator's outdated and sexist storylines.");
	tc.Add("A heartwarming coming-of-age story about a girl and her loyal dragon companion as they navigate through challenges and friendships in a magical world.");}

	{auto& tc = list.Add("Period Piece");
	tc.Add("A lavish and romantic drama set in the early 20th century about a wealthy woman torn between societal expectations and her own desires.");
	tc.Add("A historical adventure about a group of women who become highway robbers in order to survive during a time of war and famine.");
	tc.Add("A political thriller set in ancient Rome about a powerful and cunning woman using her charm and intelligence to rise to the top.");
	tc.Add("A drama about a rebellious young woman in Victorian England who finds herself at odds with society and her traditional family.");
	tc.Add("A sweeping saga about the lives and loves of a group of women from different backgrounds during the French Revolution.");}

	{auto& tc = list.Add("Musical");
	tc.Add("A modern day musical about a struggling singer who must navigate the cutthroat world of the music industry and find love along the way.");
	tc.Add("A romantic comedy about two rival a cappella groups and the unexpected romance that blossoms between their leaders.");
	tc.Add("A rock opera about a group of women in a punk band fighting against societal expectations and gender stereotypes in the 1970s.");
	tc.Add("A vibrant and colorful fantasy musical about a girl who discovers she is a princess and must use her magical powers to save her kingdom from an evil sorcerer.");
	tc.Add("A musical biopic about a legendary female musician and her rise to fame, highlighting the struggles and triumphs of her personal and professional life.");}

	{auto& tc = list.Add("Dramatic");
	tc.Add("A slow-burning and poignant drama about a woman battling drug addiction and trying to reconnect with her estranged family.");
	tc.Add("A powerful and emotional story about a female lawyer fighting for justice and facing backlash in a male-dominated legal system.");
	tc.Add("A heart-wrenching drama about a woman coming to terms with her terminal illness and making the most of her remaining time.");
	tc.Add("A tense and gripping thriller about a woman on the run from her abusive husband and fighting to protect her children.");
	tc.Add("A tragic and thought-provoking tale about a group of female soldiers dealing with the aftermath of war and the toll it takes on their relationships and mental health.");}

	{auto& tc = list.Add("Horror");
	tc.Add("A modern retelling of a classic tale about a woman cursed by a vengeful spirit and fighting to break the curse before it's too late.");
	tc.Add("A psychological horror about a woman who is haunted by her past and must confront her darkest secrets in order to survive.");
	tc.Add("A found footage style horror about a group of female friends who venture into an abandoned asylum and encounter terrifying supernatural forces.");
	tc.Add("A body horror about a woman undergoing a dangerous and experimental beauty treatment that leads to horrifying consequences.");
	tc.Add("A slasher film about a group of women on a camping trip who must fight for their lives against a masked killer in the woods.");}

	{auto& tc = list.Add("Mockumentary");
	tc.Add("A darkly comedic mockumentary about a group of feminist activists trying to infiltrate and expose a misogynistic cult.");
	tc.Add("A hilarious mockumentary about a group of women competing in a baking competition with high stakes and intense drama.");
	tc.Add("A satire of the fashion industry, following an eccentric and egotistical designer and his team of frenzied assistants as they prepare for a high-profile fashion show.");
	tc.Add("A mockumentary about a group of women trying to become the first all-female team to climb Mount Everest, with chaotic and humorous results.");
	tc.Add("A wacky and over-the-top mockumentary about a group of women running a dog grooming business and the hijinks that ensue.");}

	{auto& tc = list.Add("Documentary-style");
	tc.Add("A gripping and emotional documentary about a group of female activists fighting for women's rights in a repressive and conservative society.");
	tc.Add("A raw and honest documentary about the struggles and triumphs of a group of women trying to break into the male-dominated world of pro wrestling.");
	tc.Add("A true crime documentary about a notorious female serial killer and the events that led to her capture.");
	tc.Add("A poignant and thought-provoking documentary about the experiences of female soldiers in combat and the challenges they face when returning to civilian life.");
	tc.Add("A behind-the-scenes look at the making of an all-female action film, highlighting the hard work and dedication of the cast and crew.");}

	{auto& tc = list.Add("Experimental/Avant-garde");
	tc.Add("An abstract and surreal experimental film exploring the concept of female identity and societal expectations.");
	tc.Add("A performance art piece depicting the struggles and triumphs of a woman navigating the corporate world.");
	tc.Add("A visually stunning avant-garde film about a group of women stranded on a deserted island, grappling with isolation and survival.");
	tc.Add("A thought-provoking experimental film about a woman's journey through grief and loss, using metaphors and symbolism to convey her emotions.");
	tc.Add("A bold and daring exploration of female sexuality and desire through avant-garde dance and movement.");}

	{auto& tc = list.Add("Thriller/Suspense");
	tc.Add("A tense and gripping thriller about a woman who suspects her husband of cheating and sets out to uncover the truth.");
	tc.Add("A psychological suspense about a woman who starts receiving mysterious messages from her supposedly deceased twin sister.");
	tc.Add("A neo-noir thriller about a female detective investigating a string of murders linked to a powerful and corrupt pharmaceutical company.");
	tc.Add("A fast-paced and adrenaline-filled thriller about a woman who wakes up with no memory and must piece together the events of the previous night to clear her name.");
	tc.Add("An intense and claustrophobic thriller about a woman trapped on a malfunctioning elevator with a dangerous and unpredictable stranger.");}

	{auto& tc = list.Add("Mystery/Crime");
	tc.Add("A gripping mystery involving a group of female detectives trying to solve a string of brutal murders targeting women in their community.");
	tc.Add("A twisty and unpredictable crime drama about a woman who may or may not be guilty of a heinous crime, and the lawyer fighting to prove her innocence.");
	tc.Add("A neo-noir crime story about a female journalist investigating the disappearance of a wealthy socialite, leading her down a dark and dangerous path.");
	tc.Add("A thrilling and suspenseful murder mystery about a group of friends on a weekend getaway, discovering secrets and betrayals that could ultimately lead to murder.");
	tc.Add("A gritty and realistic crime drama about a female detective determined to take down a notorious drug lord, while facing sexism and corruption within the police force.");}


	{auto& tc = list.Add("Western");
	tc.Add("A female gunslinger seeking revenge on the men who killed her family and took her land in the Old West.");
	tc.Add("A sexy and daring love story between a female outlaw and a tough cowboy, set against the backdrop of the Wild West.");
	tc.Add("A feminist re-imagining of a classic Western, following a gang of female outlaws robbing banks and causing chaos in a male-dominated society.");
	tc.Add("A gritty and violent exploration of the hardships faced by women during the Gold Rush era, as a group of female pioneers fight for survival in a brutal and lawless wilderness.");
	tc.Add("A provocative and gritty tale of female cowboys, brothels, and shootouts in a small Western town run by powerful and corrupt men.");
	}

	{auto& tc = list.Add("War/Epic");
	tc.Add("A powerful and emotional war drama about a group of female nurses on the front lines during World War II.");
	tc.Add("A sprawling epic about a female warrior leading her people to victory against an oppressive and tyrannical empire.");
	tc.Add("A gritty and intense war film about a group of female soldiers fighting for their lives in the midst of a brutal and chaotic conflict.");
	tc.Add("A gripping war thriller about a female spy navigating dangerous territories and risking her life to gather crucial information for the Allied Forces.");
	tc.Add("An epic fantasy war film about a fierce and determined queen leading her army against a malevolent sorcerer threatening her kingdom.");}

	{auto& tc = list.Add("Slice of Life");
	tc.Add("A heartwarming and relatable slice of life film about a group of women navigating the challenges of motherhood and friendship.");
	tc.Add("A bittersweet and poignant story about a woman in her golden years, reflecting on her life, loves, and regrets.");
	tc.Add("A raw and honest portrayal of a woman struggling with addiction and trying to rebuild her life after hitting rock bottom.");
	tc.Add("A coming-of-age story about a group of young women in a small town, facing societal pressures and expectations as they try to find their place in the world.");
	tc.Add("A charming and light-hearted film about a group of female friends spending a summer at a beach house, reliving their carefree college days.");}

	{auto& tc = list.Add("Satire");
	tc.Add("A sharp and witty satire about a group of women in high society, who must confront their shallow and materialistic lifestyles.");
	tc.Add("A political satire following a female candidate's campaign for president, highlighting the absurdities and hypocrisy of the political system.");
	tc.Add("A subversive and darkly comedic take on the romantic comedy genre, featuring a female protagonist who actively tries to sabotage her own love life.");
	tc.Add("A satirical mockumentary about a group of women opening a feminist bookstore in a conservative town and dealing with backlash and resistance.");
	tc.Add("A humorous and biting satire about a group of women working in the tech industry, navigating sexism and discrimination in a male-dominated field.");}

	{auto& tc = list.Add("Surrealism");
	tc.Add("A visually stunning and dreamlike exploration of a woman's psyche and inner turmoil as she struggles with mental illness.");
	tc.Add("An emotionally charged and surreal journey of a woman trying to come to terms with her past trauma through her art.");
	tc.Add("A dark and twisted surrealist film following a group of women who become trapped in a never-ending cycle of their own nightmares.");
	tc.Add("A surrealist horror about a woman who starts to question if the strange and unsettling events happening around her are just figments of her imagination.");
	tc.Add("An experimental and surreal film about a woman who wakes up in a surreal and otherworldly landscape, trying to find her way back home.");}

	ASSERT(GetStyles().GetCount() == list.GetCount());
	return list;
}

VectorMap<String,Vector<String>>& GetStyleSafe(bool gender) {
	if (!gender)
		return GetStyleSafeMale();
	else
		return GetStyleSafeFemale();
}

VectorMap<String,Vector<String>>& GetStyleUnsafe(bool gender) {
	if (!gender)
		return GetStyleUnsafeMale();
	else
		return GetStyleUnsafeFemale();
}

VectorMap<String,Vector<String>>& GetStyleApproaches(bool unsafe, bool gender) {
	if (!unsafe)
		return GetStyleSafe(gender);
	else
		return GetStyleUnsafe(gender);
}


END_TEXTLIB_NAMESPACE
