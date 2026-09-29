# Referenties en leeslijst

Geordend per fase waarin je het nodig hebt. Niet alles hoeft vooraf gelezen te worden.

---

## RL op roguelikes — waarom het moeilijk is

Lees dit vóór fase 3, zodat de moeilijkheid geen verrassing is.

- **NetHack Learning Environment** — [paper](https://arxiv.org/abs/2006.13760) ·
  [Meta AI introductie](https://ai.meta.com/blog/nethack-learning-environment-to-advance-deep-reinforcement-learning/)
  De referentie voor waarom roguelikes een hard RL-probleem zijn: de in-game score is
  zeer sparse, waardoor onderzoekers extra en intrinsieke beloningen toevoegen — een
  scout reward per onthulde tegel, RND, count-based exploratie — om de
  exploratiebottleneck te doorbreken. Publicaties trainen er routineus met twee miljard
  environment steps op. Nuttig als kalibratie voor hoe klein je eigen game moet blijven.
- **MiniHack** — een instelbare, kleinere variant van NLE. Goed voorbeeld van hoe je
  taken versimpelt tot ze leerbaar zijn.
- **Craftax** — [paper](https://arxiv.org/abs/2402.16801)
  JAX-benchmark, expliciet ontworpen voor snelheid. Niet om te kopiëren, wel om te zien
  welke scope en throughput realistisch zijn.
- **Overzicht NLE-onderzoek** — [emergentmind](https://www.emergentmind.com/topics/nethack-learning-environment)

---

## RL-tooling

Relevant vanaf fase 3, zie [open vraag O2](beslissingen.md#o2--rl-stack).

- **PufferLib** — [docs](https://puffer.ai/docs.html)
  Gebouwd voor snelle gevectoriseerde custom environments. De PufferEnv-API ondersteunt
  environments geschreven in C native, en hun PPO-implementatie haalt in de orde van
  miljoenen steps per seconde. Hun eigen advies is ook het juiste advies voor dit
  project: begin eenvoudig, meet steps/seconde vanaf het begin, en optimaliseer pas na
  profileren.
- **CleanRL** — single-file implementaties van PPO en andere algoritmes. Trager dan
  gespecialiseerde bibliotheken, maar je kunt het in één zitting doorlezen. Uitstekend
  als leermateriaal naast een productie-implementatie.
- **Gymnasium** — de de-facto interface-conventie voor environments. Ook als je hem niet
  gebruikt, is het nuttig om de vorm te kennen (`reset`, `step`, observation space,
  action space).

**Throughput is de belangrijkste variabele van dit project.** Bij 1.000 steps/s is dit
frustrerend, bij 500.000 wordt het leuk. De klassieke valkuil is per stap over de
taalgrens springen — batch die calls.

---

## Roguelike-techniek

Relevant in fase 1, 2 en 4.

- **RogueBasin** — de verzamelplaats voor roguelike-algoritmes: field of view,
  dungeongeneratie, pathfinding.
- **Red Blob Games** (Amit Patel) — de beste uitleg die er is over grids, A*, hex- en
  vierkantsroosters en field of view. Visueel en interactief.
- **The Procedural Content Generation Book** — vrij beschikbaar. Hoofdstukken over
  search-based PCG en over het evalueren van gegenereerde content met agents zijn
  rechtstreeks relevant voor fase 5.

---

## Engine en systeemprogrammeren

Relevant in fase 1 en 2, afhankelijk van [O1](beslissingen.md#o1--implementatietaal)
en [O3](beslissingen.md#o3--rendering).

- **Game Programming Patterns** (Robert Nystrom) — gratis online. Game loop, command
  pattern, component pattern. Kort en praktisch.
- **raylib** — minimale C-bibliotheek voor window, input en 2D-rendering. Laagdrempelig
  alternatief voor SDL als de terminal niet volstaat.
- **nanobind** of **pybind11** — voor de binding van C++ naar Python in fase 3.
  nanobind is de modernere, snellere opvolger van dezelfde auteur.

De taal is C++20 ([B10](beslissingen.md#b10--c20-als-implementatietaal-was-o1)).
Nuttig daarbij:

- **cppreference.com** — de naslag voor taal en standaardbibliotheek.
- **C++ Core Guidelines** — om modern C++ te schrijven in plaats van "C met klassen".
- **doctest** — single-header testframework, gebruikt vanaf M0.
- **CMake** — de officiële *CMake Tutorial* en "Modern CMake" (cliutils.gitlab.io/modern-cmake):
  denken in targets en `target_link_libraries`, niet in globale variabelen.
- **xoshiro / splitmix64** (prng.di.unimi.it) en **PCG** (pcg-random.org) — kleine,
  snelle, portabele RNG's voor M3. Zie [B14](beslissingen.md) voor waarom niet
  `<random>`'s distributies.

Geen apart oefenproject vooraf — dit ís het oefenproject.

---

## Als het project ooit naar fase 6 gaat

- **Crafting Interpreters** (Robert Nystrom) — gratis online. Alleen relevant als er ooit
  een scriptingtaal in de engine komt, maar dan is dit de referentie.
- Literatuur over self-play en population-based training, voor het geval geleerde
  vijanden alsnog geprobeerd worden. Zie
  [beslissing B8](beslissingen.md#b8--agents-zijn-meetinstrument-geen-tegenstander)
  voor waarom dat niet het startpunt is.
