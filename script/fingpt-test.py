from transformers import AutoTokenizer, AutoModelForCausalLM

MODEL_NAME = 'TurkuNLP/gpt3-finnish-3B'

tokenizer = AutoTokenizer.from_pretrained(MODEL_NAME)
model = AutoModelForCausalLM.from_pretrained(MODEL_NAME)

def generate(prompt, temperature=0.7, max_new_tokens=50):
    input = tokenizer(prompt, return_tensors='pt')
    output = model.generate(
        **input,
        do_sample=True,
        temperature=temperature,
        max_new_tokens=max_new_tokens,
        no_repeat_ngram_size=2,
    )
    return tokenizer.decode(output[0], skip_special_tokens=True)

prompt = """Lista A: lauseita:
1. 'loista niin kirkkaasti', 'taakse, taaksepäin saa sen silmät auki', 'ratsasta sen huoran kanssa kuin upouusi hevonen'
2. 'mies, Hertta, se on villi', 'jos me paukutetaan', 'pyöritelkää sitä'
3. 'korjaus', 'nait sen', 'jos siltä tietäis sininen tie'
4. 'korjaus', 'aioin olla taiteilija', 'kuulit sen olevan makeeta'
5. 'me rakastetaan sitä kun se hymyilee', 'mä ennen myin huumeita', 'eihän sulla aina ollut tukkaa'
6. 'kohtasin sut valokuvassa', 'sä pelaat reilusti', 'laittaa sut alaspäin'
7. 'kerro mulle miksi oot vaan mulle hyvä', 'taakse, taaksepäin saa sen silmät auki', 'hänellä on staili'
8. 'sä teet mut niin täydelliseks', 'teki musta idiootin amerikka!', 'kaikki mitä mulla on'
9. 'pidä musiikkia päällä', 'sä haluat jonkun kärsivän niin kuin sä', 'huorat mun kikkelissä'
10. 'kaikki on oikeesti prameeta', 'annoin autojeni pestä greshamilla', 'kaikki sisältyy siihen'
11. 'me rakastetaan sitä kun se hymyilee', 'jos me paukutetaan', 'ja kaikki säätää'
12. 'mä ennen myin huumeita', 'otti siemeneni tärisemästä rungostani', 'hitaasti vetää takaisin'
13. 'jos siltä tietäis sininen tie', 'haluan käsi nigahtaa mixtapen', 'laittaa sut alaspäin'
14. 'pää halkesi', 'huorat mun kikkelissä', 'alhaalla viidakkomaassa'
15. 'sanoo, että jätän sut tänään', 'kaikki palasi muistiin', 'miksi piikit nukut'

Yhdistä Lista A lauseet siten, että ne rimmaja että niissä on loppusoinnut.
1. 'missä on mun alkkarit', 'murhaa kaikki nämä hirviöt': "Tää muija riisui mut enkä tiiä mis on mun alkkarit, / Hirviöt sen sängyn alla lanteen rytmil murhasin"
2. 'me ei koskaan haluttu', 'me vain halutaan olla': "Me ei koskaan haluttu elää täl tavalla, / Me vain halutaan olla vapaita, ja karata kaikelta pahalta."
3. 'loista niin kirkkaasti', 'taakse, taaksepäin saa sen silmät auki', 'ratsasta sen huoran kanssa kuin upouusi hevonen':"""

prompt =  """2 lausetta:
1. me ei koskaan haluttu
2. me vain halutaan olla

Yhdistä 2 lausetta siten, että ne rimmaa:
Me ei koskaan haluttu elää täl tavalla,
Me vain halutaan olla vapita, ja karata kaikelta pahalta.


3 lausetta:
1. korjaus
2. nait sen
3. jos siltä tietäis sininen tie

Yhdistä 3 lausetta siten, että ne rimmaa:"""

print(generate(prompt))
