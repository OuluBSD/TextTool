#include "ToolCore.h"
#include "ProtectedCommon.h"


BEGIN_TEXTLIB_NAMESPACE



const Index<String>& GetTypecasts() {
	thread_local static Index<String> list;
	if (list.IsEmpty()) {
		list.Add("Heartbroken/lovesick");
		list.Add("Rebel/anti-establishment");
		list.Add("Political activist");
		list.Add("Social justice advocate");
		list.Add("Party/club");
		list.Add("Hopeful/dreamer");
		list.Add("Confident/empowered");
		list.Add("Vulnerable/raw");
		list.Add("Romantic/love-driven");
		list.Add("Failure/loser");
		list.Add("Spiritual/faithful");
		list.Add("Passionate/determined");
		list.Add("Reflective/self-reflective");
		list.Add("Witty/sarcastic");
		list.Add("Melancholic/sad");
		list.Add("Humble/down-to-earth");
		list.Add("Charismatic/charming");
		list.Add("Resilient/overcoming adversity");
		list.Add("Carefree/joyful");
		list.Add("Dark/mysterious");
		list.Add("Comical/humorous");
		list.Add("Controversial/provocative");
		list.Add("Nostalgic/sentimental");
		list.Add("Wise/philosophical");
		list.Add("Angry/outspoken");
		list.Add("Calm/peaceful.");
		list.Add("Confident/self-assured");
		list.Add("Self-destructive/self-sabotaging");
		list.Add("Hopeful/optimistic");
		list.Add("Fearful/anxious");
		list.Add("Eccentric/quirky");
		list.Add("Sensitive/emotional");
		list.Add("Bitter/resentful");
		list.Add("Unique/nonconformist");
		list.Add("Free-spirited/nonconformist");
		list.Add("Sultry/seductive");
		list.Add("Inspirational/motivational");
		list.Add("Authentic/real");
		list.Add("Mysterious/enigmatic");
		list.Add("Carefree/bohemian");
		list.Add("Street-smart/tough");
		list.Add("Romantic/idealistic");
		list.Add("Nurturing/motherly");
		list.Add("Dark/tormented");
		list.Add("Remorseful/regretful");
		list.Add("Bold/brave");
		list.Add("Outcast/rebel");
		list.Add("Lost/disconnected");
		list.Add("Tough/badass");
		list.Add("Sincere/genuine");
		list.Add("Honest/vulnerable");
		list.Add("Innocent/naive");
		list.Add("Bold/risk-taking");
	}
	return list;
}

int GetTypecastCount() {
	return GetTypecasts().GetCount();
}

const Index<String>& GetProfiles() {
	thread_local static Index<String> list;
	if (list.IsEmpty()) {
		// "singer is ..."
        list.Add("a third-party observer/commentator");
        list.Add("expressing personal emotions/thoughts");
        list.Add("conveying a message or lesson");
        list.Add("embodying a character or persona");
        list.Add("a storyteller or messenger for a community or culture");
        list.Add("reflecting on past experiences or memories");
        list.Add("interpreting or analyzing the scripts for the listener");
        list.Add("challenging societal norms or addressing social issues");
        list.Add("invoking a particular mood or atmosphere through vocals");
        list.Add("having a dialogue with another singer");
        list.Add("having a dialogue with the audience");
        list.Add("using abstract or poetic language to convey feelings or ideas");
        list.Add("highlighting the beauty or poeticism of the scripts through their performance");
        list.Add("asking questions and exploring different perspectives on the topic of the scripts");
        list.Add("using irony or satire to convey a message or make a statement");
        list.Add("evoking nostalgia or longing through the scripts");
        list.Add("using personal experiences to give depth and authenticity to the scripts ");
        list.Add("using humor or wit to engage with the scripts");
        list.Add("challenging the listener's perspective or beliefs with the scripts");
        list.Add("using vocal techniques or styles to add layers of meaning to the scripts");
        list.Add("creating a sense of intimacy or connection through their performance of the scripts");
        list.Add("breaking societal expectations and norms through their interpretation of the scripts");
        list.Add("offering a unique perspective on a commonly addressed topic in the scripts");
        list.Add("creating a soundtrack or anthem for a specific group or community");
        list.Add("using repetition or emphasis to emphasize the importance of the scripts");
        list.Add("using double entendres or wordplay to add depth and complexity to the scripts ");
        list.Add("reflecting on personal growth or transformation through the scripts");
        list.Add("embodying a specific emotion or feeling portrayed in the scripts");
        list.Add("representing a marginalized or underrepresented group through the scripts");
        list.Add("using imagery or metaphors to convey a deeper meaning behind the scripts");
        list.Add("expressing vulnerability or raw emotion through the scripts");
        list.Add("narrating a specific event or experience through the scripts");
        list.Add("using the scripts to convey a sense of empowerment or strength");
        list.Add("engaging in introspection and self-reflection through the scripts");
        list.Add("confronting personal demons or struggles in the scripts ");
        list.Add("using the scripts to express social or cultural commentary");
	}
	return list;
}

int GetProfileCount() {
	return GetProfiles().GetCount();
}





const Vector<ContentType>& GetContrasts() {
	thread_local static Vector<ContentType> list;
	if (list.IsEmpty()) {
		list.Add().Set("Seductive intro", "a seductive and sultry melody draws the listener in", "the scripts talk about a passionate and intense relationship", "the mood shifts as the singer realizes they are not truly in love");
		list.Add().Set("Rise and fall", "the beat builds and intensifies, creating a sense of excitement and anticipation", "the scripts tell a story of overcoming obstacles and achieving success", "the energy drops suddenly and the singer reflects on the sacrifices and struggles that came with their success");
		list.Add().Set("Fun and games", "a carefree and lively melody sets the tone for a carefree party anthem", "the scripts are about enjoying life and living in the moment", "the party comes to an end and the reality of responsibilities and consequences sink in");
		list.Add().Set("Love at first sight", "a romantic and dreamy melody introduces the concept of falling in love at first sight", "the scripts describe the intense feelings and desires that come with falling for someone instantly", "the singer wakes up from the fantasy and realizes");
		list.Add().Set("Struggle and triumph", "a slower and melancholic melody sets the scene for a character facing challenges and adversity", "the scripts depict the struggles and hardships they have faced", "the pace picks up and the music becomes more triumphant as the character overcomes their struggles and achieves success");
		list.Add().Set("Ups and downs", "a catchy and upbeat melody reflects the highs of a new relationship", "the scripts delve into the challenges and conflicts that arise within the relationship", "the music slows down as the couple try to work through their problems and find a resolution");
		list.Add().Set("Escape to paradise", "a tropical and laid-back beat transports the listener to a paradise destination", "the scripts describe a desire to escape from reality and find solace in a beautiful location", "the singer comes back to reality and faces the consequences of leaving everything behind");
		list.Add().Set("Rebellious spirit", "a rebellious and edgy guitar riff sets the rebellious tone of the song", "the scripts speak of breaking rules and societal expectations", "the song ends with the realization that rebellion can have consequences");
		list.Add().Set("Broken and mended", "a somber and melancholic melody reflects a heartbroken state", "the scripts describe the pain and sadness of a broken relationship", "the tone shifts as the singer begins to heal and move on from the heartbreak");
		list.Add().Set("Chase your dreams", "an uplifting and motivational melody encourages listeners to chase their dreams", "the scripts tell a story of overcoming obstacles and pursuing one's passions", "the song concludes with a sense of fulfillment and the realization that the journey towards achieving dreams is never-ending");
		list.Add().Set("Dark secrets", "a haunting and mysterious introduction sets the tone for secrets and deceit", "the scripts reveal dark secrets and hidden motives among the characters", "the song ends with a sense of betrayal and the consequences of keeping secrets");
		list.Add().Set("Rags to riches", "a humble and modest melody represents the beginnings of a character's journey", "the scripts describe the climb to success and wealth", "the music becomes more grandiose as the character achieves their dreams and reflects on their journey");
		list.Add().Set("Lost and found", "a haunting and melancholic melody portrays a sense of being lost and alone", "the scripts depict a journey of self-discovery and finding one's place in the world", "the music becomes more uplifting as the character finds a sense of belonging and purpose");
		list.Add().Set("Ignite the fire", "an energetic and intense beat sparks excitement and passion", "the scripts describe the power and intensity of a new love or passion", "the music dies down as the flame fades and the singer is left with the memories of the passion that once consumed them");
		list.Add().Set("From the ashes", "a slow and mournful melody sets the scene for a character who has hit rock bottom", "the scripts depict the struggles and hardships they have faced", "the music picks up as the character rises from the ashes and rebuilds their life" );
		list.Add().Set("Fame and fortune", "a flashy and upbeat melody represents the allure of fame and fortune", "the scripts describe the glamorous lifestyle and perks that come with success", "the song ends with a cautionary tale about the emptiness and pitfalls of a life solely focused on money and fame");
		list.Add().Set("Healing in the darkness", "a haunting and ethereal melody reflects a state of darkness and pain", "the scripts speak of finding light and healing in the darkest times", "the music builds to a triumphant and uplifting finale as the singer finds strength and hope in their struggles");
		list.Add().Set("City lights and lonely nights", "a bustling and energetic beat represents the excitement of the city at night", "the scripts tell a story of chasing dreams and living life to the fullest in the city", "the song ends with a sense of loneliness and longing for something more meaningful outside of the fast-paced city life");
		list.Add().Set("Breaking the mold", "a unique and unconventional melody sets the tone for breaking the norm", "the scripts describe defying expectations and being true to oneself", "the song ends with a sense of liberation and empowerment as the singer embraces their individuality");
		list.Add().Set("Haunted by the past", "a haunting and eerie melody reflects the weight of a character's past traumas", "the scripts delve into the pain and struggles of moving on from the past", "the music becomes more hopeful as the character learns to let go and move forward");
		list.Add().Set("Wild and free", "a carefree and adventurous melody embodies the thrill of living life on the edge", "the scripts describe the rush and excitement of taking risks and living in the moment", "the song concludes with a reminder that with freedom comes consequences and responsibilities");
		list.Add().Set("Clash of opinions", "a catchy and upbeat melody sets the tone for a heated argument", "the scripts depict conflicting opinions and viewpoints", "the song ends with the understanding that sometimes it's best to agree to disagree and move on" );
		list.Add().Set("Long distance love", "a soft and tender melody represents the longing and distance in a relationship", "the scripts tell a story of the struggles and sacrifices of maintaining a long distance love", "the song ends with a sense of hope and determination to make the relationship work");
		list.Add().Set("Finding inner strength", "a slow and contemplative melody represents a character facing inner struggles", "the scripts speak of finding courage and strength from within to overcome challenges", "the song crescendos as the singer embraces their inner strength and triumphs over their struggles");
		list.Add().Set("Living a double life", "a mysterious and seductive beat sets the stage for a character leading a secretive life", "the scripts tell the story of juggling two separate identities and the dangers that come with it", "the song concludes with the realization that living a lie is destructive and unsustainable");
		list.Add().Set("Caught in the spotlight", "a bright and flashy melody reflects the thrill of being in the spotlight", "the scripts depict the pressure and challenges of fame and constantly being in the public eye", "the music slows down as the singer reflects on the toll fame has taken on their personal life");
		list.Add().Set("Love and war", "a powerful and intense beat represents the passionate and tumultuous nature of love", "the scripts depict a couple's constant battle and struggle to make their relationship work", "the song ends with a bittersweet realization that love can be both beautiful and painful");
		list.Add().Set("The art of letting go", "a slow and somber melody sets the tone for learning to let go", "the scripts describe the struggles of moving on and leaving the past behind", "the music builds to a hopeful and empowering finale as the singer finally finds the strength to let go");
		list.Add().Set("Living in the moment", "an upbeat and carefree melody represents living life with no regrets", "the scripts encourage taking chances and embracing every moment", "the song ends with a reminder to cherish the present and not dwell on the past or worry about the future");
		list.Add().Set("Conquering fears", "a tense and ominous melody reflects the fear and anxiety a character faces", "the scripts speak of overcoming fears and finding courage to face them", "the music becomes triumphant and uplifting as the character conquers their fears and grows stronger" );
		/*list.Add().Set("Heart vs. Mind", "a gentle and emotional melody sets the stage for a character torn between their heart and their logical mind", "the scripts describe the internal struggle between following one's emotions and making rational decisions", "the song ends on a reflective note as the character finds a balance between their heart and mind");
		list.Add().Set("Surviving the storm", "a stormy and intense melody represents facing difficult and challenging times", "the scripts speak of resilience and perseverance through tough situations", "the music calms down as the singer finds strength and hope in surviving the storm");
		list.Add().Set("Living a lie", "a dark and deceptive melody reflects the deception in a character's life", "the scripts depict the consequences and chaos that come with living a lie", "the song ends with the realization that living a lie can destroy relationships and one's own sense of self");
		list.Add().Set("Forgotten memories", "a melancholic and haunting melody sets the scene for a character's forgotten memories", "the scripts unravel the mystery and pain behind the forgotten memories", "the music becomes reflective and haunting as the singer realizes the true impact of their forgotten memories on their life");
		list.Add().Set("Breaking free", "a powerful and empowering melody reflects the desire to break free from constraints and expectations", "the scripts describe the journey of breaking through obstacles and finding independence", "the song ends on a triumphant note as the character finally breaks free and finds freedom and happiness" );
		list.Add().Set("Fake love", "a catchy and upbeat melody represents the facade of a fake love", "the scripts reveal the deceit and manipulation in a fake relationship", "the song ends with the realization of the emptiness and pain of a love built on lies and false promises");
		list.Add().Set("Strangers in love", "a soft and romantic melody represents the initial attraction and connection between two strangers", "the scripts follow the journey of getting to know each other and falling in love", "the song ends with the uncertainty and fear of whether their love will last or if they were always destined to be strangers");
		list.Add().Set("Guy in club", "an upbeat and danceable melody sets the scene for a night out at the club", "the scripts depict the excitement and charm of a guy at the club", "the song ends on a reflective note as the singer realizes the emptiness and lack of substance in these interactions");
		list.Add().Set("Angels and demons", "a haunting and eerie melody sets the tone for the duality within a character", "the scripts describe the battle between good and evil within oneself", "the music becomes more chaotic as the character struggles to find balance between their angels and demons");
		list.Add().Set("Living on the edge", "a fast-paced and thrilling beat represents the risky and dangerous lifestyle of living on the edge", "the scripts depict the rush and adrenaline that comes with constantly pushing boundaries", "the song ends with a sense of exhaustion and questioning if the thrill is worth the consequences");*/
	}
	return list;
}

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




VectorMap<String,Vector<String>>& GetTypecastSingers(bool gender) {
	if (!gender)
		return GetTypecastSingersMale();
	else
		return GetTypecastSingersFemale();
}

VectorMap<String,Vector<String>>& GetTypecastRappers(bool gender) {
	if (!gender)
		return GetTypecastRappersMale();
	else
		return GetTypecastRappersFemale();
}

VectorMap<String,Vector<String>>& GetTypecastArtists(bool rapper, bool gender) {
	if (!rapper)
		return GetTypecastSingers(gender);
	else
		return GetTypecastRappers(gender);
}





const Vector<String>& InlineRapperList() {
	static Vector<String> v;
	static Vector<String> list;
	if (list.IsEmpty()) {
		list.Add("Eminem");
		list.Add("Kendrick Lamar");
		list.Add("MF DOOM");
		list.Add("Big L");
		list.Add("Aesop Rock");
		list.Add("Busta Rhymes");
		list.Add("Earl Sweatshirt");
		list.Add("Tech N9ne");
		list.Add("Logic");
		list.Add("Kool G Rap ");
		list.Add("Royce da 5'9");
		list.Add("Rakim");
		list.Add("Black Thought");
		list.Add("Canibus");
		list.Add("Blackalicious");
		list.Add("Danny Brown");
		list.Add("Big Pun");
		list.Add("GZA/Genius");
		list.Add("R.A. the Rugged Man");
		list.Add("Jean Grae");
	}
	return list;
}

const Vector<String>& OtherRapperList() {
	static Vector<String> list;
	if (list.IsEmpty()) {
		list.Add("J. Cole");
		list.Add("Nicki Minaj");
		list.Add("Tyler, The Creator");
		list.Add("Chance the Rapper");
		list.Add("Mac Miller");
		list.Add("Travis Scott");
		list.Add("J.I.D.");
		list.Add("Tierra Whack");
		list.Add("Noname");
		list.Add("Vic Mensa");
		list.Add("A$AP Rocky");
		list.Add("Lil Wayne");
		list.Add("Method Man");
		list.Add("Ghostface Killah");
		list.Add("Jay-Z");
		list.Add("Andre 3000");
		list.Add("Nas");
		list.Add("Lauryn Hill");
	}
	return list;
}

END_TEXTLIB_NAMESPACE


