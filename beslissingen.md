# Beslissingslogboek

Elke beslissing met de reden erbij, zodat later-jij weet waarom iets zo is en niet
opnieuw dezelfde discussie voert. Beslissingen mogen omgegooid worden — noteer dan
waarom.

Laatst bijgewerkt: 29 september 2026

---

## Vastgelegd

### B1 — Het doel is leren, niet geld

**Beslissing:** het project wordt beoordeeld op wat het je leert, niet op wat het
opbrengt.

**Waarom:** de uitgangsvraag was aanvankelijk "iets dat later geld kan opbrengen", maar
bij nader inzien is het werkelijke motief technisch scherp blijven tijdens een opleiding
die inhoudelijk elders ligt. Dat zijn verschillende projecten met verschillende
optimalisaties.

**Gevolg:** marktomvang, monetisatie en doelgroep zijn geen selectiecriteria meer. De
faalmodus verschuift van "niemand wil dit" naar twee andere: iets bouwen dat je al kunt,
of nooit bij de laatste 20% komen. Daar is de fasering op ontworpen.

---

### B2 — Een game, geen SaaS

**Beslissing:** een game, niet een softwareproduct voor een niche.

**Waarom:** de leer-dichtheid per uur ligt hoger. Real-time loops, rendering, performance
onder een frame budget, systemen die op elkaar inwerken, wiskunde — dat zie je niet in
een gewone applicatiestack. Motivatie is bovendien de schaarse grondstof bij een
sideproject naast een opleiding: een project dat je leuk vindt maak je af, een
verstandig project laat je in februari vallen.

**Alternatieven verworpen:** zie [`brainstorm.md`](brainstorm.md). De SaaS-ideeën zijn
niet slecht, ze passen alleen bij een ander doel.

---

### B3 — Eigen engine, geen Unity of Godot

**Beslissing:** simulatie en renderer worden zelf geschreven.

**Waarom:** dit is geen esthetische keuze maar een technische vereiste. RL heeft nodig:
volledig deterministische simulatie, een headless modus zonder rendering, en de
mogelijkheid om duizenden kopieën parallel op maximale snelheid te draaien. Kant-en-klare
engines geven je die drie dingen niet of alleen met veel pijn.

**Bonus:** dezelfde eigenschappen leveren replays, save/load en deterministische tests
gratis op.

**Grens:** dit wordt *deze game's* engine, geen algemeen framework. Zie het
waarschuwingssignaal in [`projectplan.md`](projectplan.md#4-hoe-je-weet-dat-het-misgaat).

---

### B4 — Genre: turn-based grid roguelike

**Beslissing:** turn-based, grid-based, ASCII of eenvoudige tiles.

**Waarom:**
- Mechanisch rijk maar visueel eenvoudig — geen artiest nodig
- ASCII is in dit genre authentiek in plaats van een compromis
- Turn-based en discreet sluit natuurlijk aan op de RL-formulering
- Kleine, afgebakende stappen passen bij versnipperde werktijd

---

### B5 — Simulatie, rendering en input strikt gescheiden

**Beslissing:** de simulatie weet niets van rendering of input. Vanaf regel één.

**Waarom:** dit is de enige beslissing die nu al goed moet. Als rendering in de game
logic verweven raakt, kan er in fase 3 niet meer headless getraind worden zonder alles
te herschrijven. Alle andere keuzes zijn later nog om te gooien.

---

### B6 — Deterministisch en seeded vanaf het begin

**Beslissing:** zelfde seed plus zelfde acties geeft altijd dezelfde eindtoestand. Er
komt een test die dat bewijst, in fase 1.

**Waarom:** reproduceerbare training, bruikbare bugreports, replays, en de mogelijkheid
om een run exact opnieuw af te spelen. Achteraf determinisme inbouwen is veel duurder
dan het vanaf het begin afdwingen.

---

### B7 — Klein beginnen, pas uitbreiden als het werkt

**Beslissing:** eerste trainbare versie is volledig observeerbaar, met een kleine
actieruimte (vier richtingen, aanvallen, wachten) en een vaste map.

**Waarom:** roguelikes combineren alles wat RL moeilijk maakt: procedurele generatie,
sparse reward, lange horizon, partiële observeerbaarheid, grote actieruimte. De
NetHack Learning Environment bestaat als onderzoeksbenchmark precies daarom, en
publicaties daar trainen met miljarden environment steps. Dat budget is er hier niet.
Fog of war dwingt bovendien naar recurrente policies — te vroeg.

**Gevolg:** curriculum learning is het vangnet. 5×5 lege kamer → één vijand → gangen →
grotere map.

---

### B8 — Agents zijn meetinstrument, geen tegenstander

**Beslissing:** getrainde vijanden zijn géén fundament van het project. De agent wordt
gebruikt om levels te evalueren.

**Waarom:** het oorspronkelijke idee was om ook vijanden via RL te trainen voor de
moeilijkheidsgraad. Drie bezwaren:

1. Games gebruiken behavior trees, utility AI en GOAP niet uit achterstand maar omdat
   ontwerpers controle en leesbaarheid nodig hebben. RL-vijanden zijn onvoorspelbaar,
   moeilijk te tunen en vinden degenererende exploits.
2. Het doel van vijand-AI is niet winnen maar interessant verliezen. Een onverslaanbare
   vijand maak je in vijf regels code; de moeilijkheid zit in *leuk*, niet in *sterk*.
3. Speler en vijanden tegelijk laten leren is self-play met een niet-stationair
   doelwit. Dat is onderzoekswerk, geen startpunt.

**In plaats daarvan:** genereer een level, laat de agent er honderden keren doorheen
lopen, en gebruik win rate, gemiddeld aantal beurten en resterende HP als objectief
moeilijkheidslabel. Daarmee kun je onspeelbare en triviale levels wegfilteren, generatie
sturen op een doelmoeilijkheid, en balanswijzigingen meten in plaats van gokken.

Dit is bestaande, gebruikte techniek (agent-based evaluatie van procedureel gegenereerde
content), origineler dan getrainde vijanden, en wél haalbaar.

**Niet definitief afgeschreven:** geleerde vijanden mogen terugkomen in fase 6, als
experiment.

---

### B9 — Elke fase is op zichzelf af

**Beslissing:** het project wordt zo gefaseerd dat stoppen na elke fase iets bruikbaars
oplevert.

**Waarom:** engine, game en RL zijn elk op zich een jaar werk. Gestapeld is dat het
klassieke recept om nooit iets af te maken. De laatste 20% van een project bevat
bovendien het grootste deel van het leereffect: edge cases, performance, refactoren
terwijl er al gewicht op de structuur staat.

---

## Nog open

### O1 — Implementatietaal

**Kandidaten:** Rust, C, C++, Zig.

**Afwegingen**
- *Rust* — grootste leerwinst, moderne toolchain, uitstekende bindings naar Python via
  PyO3. Wel een steile leercurve bovenop een nieuw domein.
- *C* — sluit het beste aan bij RL-tooling die native C-environments ondersteunt;
  eenvoudige FFI. Minder comfortabel, meer voetangels.
- *C++* — bekend terrein voor veel ingenieurs, maar daardoor ook minder leerwinst.
- *Zig* — interessant, maar een klein ecosysteem betekent dat je meer zelf uitzoekt.

**Regel die hier geldt:** verander één of twee dingen tegelijk. Nieuwe taal én nieuw
domein kan. Daar C nog bij nemen omdat de RL-bibliotheek dat prettig vindt, is een
verandering te veel.

**Te beslissen:** vóór fase 1.

---

### O2 — RL-stack

**Kandidaten**
- PufferLib met PyTorch — gebouwd voor snelle gevectoriseerde custom environments,
  ondersteunt environments in C, en hun PPO haalt in de orde van miljoenen steps per
  seconde
- CleanRL — single-file implementaties, minder snel maar veel leerzamer om te lezen
- Een JAX-aanpak in de stijl van Craftax — extreem snel, maar dwingt tot een functionele
  simulatiestijl die botst met "een engine bouwen"
- Eigen PPO — maximale leerwinst, maximale kans om vast te lopen op een subtiele bug

**Te beslissen:** aan het begin van fase 3. Overweging: begin met een bestaande
implementatie zodat een niet-werkende agent aan jóuw environment ligt en niet aan jouw
PPO. Eigen implementatie kan later als fase 6.

---

### O3 — Rendering

**Kandidaten:** terminal/ASCII, raylib, SDL, of rechtstreeks een GPU-API zoals wgpu.

**Afweging:** terminal is het snelst tot een speelbare build en volstaat volledig voor
fase 2. Een echte renderer is leerzamer maar kan ook fase 6 zijn. Beslissing hangt af
van of rendering een leerdoel is of een middel.

**Te beslissen:** begin fase 2.

---

### O4 — Observatie-representatie voor de agent

Grid-tensor (kanalen per entiteitstype, CNN-policy) versus een compacte feature vector
(MLP-policy). Beïnvloedt trainingssnelheid en generalisatie sterk.

**Te beslissen:** fase 3, waarschijnlijk na experimenteren met beide.

---

### O5 — Reward-ontwerp

Hoeveel shaping is nodig? Kandidaten: beloning voor nieuwe tegels, afstand tot doel,
straf per beurt, intrinsieke exploratiebeloningen. Te veel shaping levert een agent op
die de shaping optimaliseert in plaats van het spel.

**Te beslissen:** empirisch, in fase 3. Begin minimaal en voeg toe zodra blijkt dat er
niets geleerd wordt.

---

### O6 — Publiek of privé

Open source vanaf het begin, een publieke devlog, of alles privé tot er iets staat?

Overweging: een devlog dwingt tot expliciteren en dat is een leerdoel op zich. Maar
publiek werken voegt ook druk toe aan een project dat expliciet geen deadline heeft.

**Te beslissen:** geen haast.

---

### O7 — Inzet als lesmateriaal

Een agent die zichtbaar leert spelen is uitstekend demonstratiemateriaal voor het
vakdidactische werk. Waard om in het achterhoofd te houden bij fase 5 en 6, maar geen
ontwerpcriterium.

**Te beslissen:** niet urgent.
