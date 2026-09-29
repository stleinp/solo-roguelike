# Projectplan

Status: **fase 0 — planning**
Laatst bijgewerkt: 29 september 2026

---

## 1. Doel

Technisch scherp blijven en bewust nieuwe domeinen leren, in een jaar waarin de
opleiding inhoudelijk elders ligt. Het project is geslaagd als er aan het eind van het
academiejaar een speelbare game staat waar een getrainde agent doorheen loopt, en als
er onderweg dingen geleerd zijn die niet uit een tutorial komen.

**Expliciete niet-doelen:** omzet, gebruikersaantallen, een publiek product.

### Wat er geleerd moet worden

| Domein | Waarom nieuw of nuttig |
|---|---|
| Engine-architectuur | Game loop, state management, scheiding sim/render, determinisme |
| Systeemprogrammeren | Een taal dichter bij de machine dan de gebruikelijke stack |
| Performance | Throughput meten en optimaliseren als eerste-orde-eis, niet als bijzaak |
| FFI / bindings | Simulatie in een systeemtaal, training in Python |
| Reinforcement learning | Reward-ontwerp, curriculum, exploratie, evaluatie |
| Procedurele generatie | Generatie *plus* automatische evaluatie van het resultaat |

---

## 2. Afbakening

### Binnen scope

- Turn-based, grid-based roguelike
- ASCII of eenvoudige tiles (geen art-pijplijn, geen artiest nodig)
- Single player, single agent
- Volledig observeerbaar in de eerste versies
- Kleine actieruimte: vier richtingen, aanvallen, wachten

### Buiten scope (voorlopig)

- Real-time gameplay, 3D, physics
- Fog of war / partiële observeerbaarheid — dwingt naar recurrente policies en maakt
  training aanzienlijk moeilijker
- Inventaris, items, magie — blaast de actieruimte op
- Multi-agent of self-play
- Een algemene, herbruikbare engine
- Netwerken, multiplayer, persistentie over sessies

Alles in de tweede lijst mag later terugkomen, maar pas nadat de fase ervoor werkt.

### Waarom deze afbakening

Roguelikes zijn een berucht moeilijk RL-domein: procedurele generatie, sparse reward,
lange horizon, partiële observeerbaarheid en een grote actieruimte tegelijk.
Onderzoeksomgevingen zoals de NetHack Learning Environment bestaan precies omdát het zo
moeilijk is, en publicaties daar trainen routineus met miljarden environment steps. Dat
budget is er hier niet. De game moet dus klein genoeg blijven dat leren überhaupt
mogelijk is op één machine.

---

## 3. Fasering

Tijdsinschattingen gaan uit van ongeveer 5 à 8 uur per week, met onderbrekingen.
Elke fase is op zichzelf af: stoppen na een fase levert altijd iets bruikbaars op.

### Fase 1 — Deterministische headless kern

**Duur:** ~3–5 weken

Bouw de simulatie zonder enige vorm van rendering. Een vaste kleine map, een speler,
één vijand, vijf acties. Alles gestuurd door een expliciete seed.

**Deliverables**
- Simulatiekern met een `step(action) -> state` interface
- Seeded RNG, volledig deterministisch
- Een testsuite die determinisme bewijst: zelfde seed + zelfde acties = zelfde eindstaat
- Een benchmark die steps/seconde print

**Klaar wanneer**
- Een script kan duizenden willekeurige episodes draaien zonder rendering
- Determinismetest slaagt betrouwbaar
- Er is een getal voor throughput (het startpunt, niet het doel)

**Wat je leert:** taal en toolchain, game state modelleren, determinisme als
ontwerpprincipe, benchmarking.

**Risico:** de neiging om hier al een "engine" te bouwen. Bouw de simulatie van *deze*
game, niet een framework.

---

### Fase 2 — Speelbaar maken

**Duur:** ~2–4 weken

Renderer en input erbovenop, strikt als een aparte laag die de simulatie leest en
acties terugstuurt. De simulatie mag niets weten van rendering.

**Deliverables**
- Renderer (terminal of eenvoudige tiles)
- Input handling, main loop
- Een build die een mens kan spelen

**Klaar wanneer** iemand anders het kan opstarten en spelen zonder uitleg.

**Wat je leert:** laagscheiding in de praktijk, input/timing, de eerste echte
architectuurdruk.

**Dit is de eerste mijlpaal die telt.** Vanaf hier heb je iets te tonen, ook als het
project daarna stilvalt.

---

### Fase 3 — Trainbare environment en eerste agent

**Duur:** ~4–8 weken — de zwaarste fase

Maak de simulatie aanroepbaar vanuit de RL-stack en train een eerste agent op een vaste
kleine map.

**Deliverables**
- Binding van simulatie naar de trainingsomgeving
- Gevectoriseerde environments: N kopieën parallel in één proces, batchgewijs gestapt
- Observatie- en actie-representatie
- Een werkende PPO-baseline
- Loggen van throughput, reward, win rate

**Klaar wanneer** de agent op een vaste map betrouwbaar het doel haalt, meetbaar beter
dan een willekeurige policy.

**Wat je leert:** FFI, vectorisatie en batching, reward shaping, curriculum learning,
en vooral: waarom RL vooral debuggen is.

**Risico's**
- *Throughput.* Bij 1.000 steps/s is dit frustrerend, bij 500.000 wordt het leuk. Per
  stap over de taalgrens springen is de klassieke valkuil; batch de calls.
- *Sparse reward.* Alleen belonen bij het bereiken van het doel levert een agent op die
  maandenlang niets doet. Reken op shaping (beloning voor nieuwe tegels, afstand tot
  doel, straf per beurt) of intrinsieke beloningen.
- *Te snel opschalen.* Voeg pas generatie, items of fog of war toe als dit werkt.

**Curriculum als vangnet:** 5×5 lege kamer → één vijand → gangen → grotere map. Als de
agent op de simpelste variant niks leert, ligt het aan de opzet, niet aan de
hyperparameters.

---

### Fase 4 — Procedurele generatie en generalisatie

**Duur:** ~4–6 weken

**Deliverables**
- Levelgenerator (kamers, gangen, plaatsing van vijanden en doel)
- Train/test-split op seeds: trainen op de ene set, evalueren op onbekende seeds
- Rapport van generalisatieprestatie

**Klaar wanneer** de agent op ongeziene maps aantoonbaar beter presteert dan willekeurig.

**Wat je leert:** procedurele generatie, overfitting op seeds, evaluatiemethodologie.

---

### Fase 5 — Agent als playtester

**Duur:** ~4–6 weken — de inhoudelijke kern van het project

Draai de getrainde agent honderden keren per gegenereerd level en gebruik de resultaten
als objectief moeilijkheidslabel.

**Deliverables**
- Evaluatiepijplijn: level in, statistieken uit (win rate, gemiddeld aantal beurten,
  resterende HP, doodsoorzaak)
- Automatisch wegfilteren van onspeelbare en triviale levels
- Generatie die stuurt op een doelmoeilijkheid
- Een balansdashboard: het effect van een aanpassing wordt een getal in plaats van een
  gevoel

**Klaar wanneer** je een parameter in de generator kunt wijzigen en binnen enkele
minuten kwantitatief ziet wat dat met de moeilijkheidsgraad doet.

**Waarom dit interessanter is dan getrainde vijanden:** zie
[beslissing B8](beslissingen.md#b8--agents-zijn-meetinstrument-geen-tegenstander).

---

### Fase 6 — Open einde

Pas relevant als fase 5 draait. Mogelijke richtingen:

- Adaptieve moeilijkheidsgraad tijdens het spelen, gestuurd door de difficulty-oracle
- Uitbreiding van de game: items, fog of war, meer vijandtypes — met de agent als
  regressietest op balans
- Pas hier: geleerde vijanden of self-play, als experiment en niet als fundament
- Visualisatie van het leerproces (uitstekend lesmateriaal)
- Eigen PPO-implementatie ter vervanging van de bibliotheek, puur om te begrijpen

---

## 4. Hoe je weet dat het misgaat

Waarschuwingssignalen, in volgorde van ernst:

1. **Na twee maanden is er nog niets speelbaars.** Meestal omdat fase 1 uitdijde tot
   engine-bouw. Snijd scope, ga naar fase 2.
2. **De engine wordt algemeen.** Zodra er woorden als "plugin", "asset pipeline" of
   "herbruikbaar" vallen zonder dat deze game ze nodig heeft: stoppen.
3. **Training begint voordat de game speelbaar is.** Dan debug je twee onbekenden
   tegelijk.
4. **Throughput wordt pas gemeten als het traag aanvoelt.** Te laat; meet vanaf fase 1.
5. **Hyperparameters worden gestemd voordat de baseline werkt.** Als een simpele agent
   op een triviale map niets leert, ligt het aan reward of representatie.
6. **Het is geen plezier meer.** Dit project heeft geen deadline en geen klant. Dat is
   het enige echte succescriterium.

---

## 5. Onderhoud van deze documenten

- `beslissingen.md` bijwerken zodra een open vraag beslist wordt — met de reden, niet
  alleen de uitkomst
- `README.md` statusregel bijwerken bij elke faseovergang
- `projectplan.md` herzien na fase 3; de inschattingen daarna zijn het minst betrouwbaar
