# PDF → C++ Messages — Workflow (style maison)

Workflow VS Code / Copilot pour générer des **structures de messages C++ au style maison** à partir d'un **PDF de spécification** (ICD, protocole binaire de marché, etc.).

## Style maison généré

Pour chaque message, le workflow produit :

```cpp
#pragma pack(push, 1)
struct PostTradeStruct
{
    uint8   m_MDUpdateAction;
    int64   m_MsgSeqNum;
    char    m_SecurityID[12];
    Decimal m_MDEntryPx;
    // ...
};
#pragma pack(pop)

class PostTrade
{
    MessageHeader   const   m_MessageHeader;
    PostTradeStruct const & m_Struct;
public:
    static constexpr const uint16  TYPE = 201;
    static constexpr const char*   NAME = "PostTrade";
    static const unsigned int      SIZE = MessageHeader::SIZE + sizeof(PostTradeStruct);

    explicit PostTrade(InputData const & data)
    : m_MessageHeader(data)
    , m_Struct(*reinterpret_cast<const PostTradeStruct*>(data.data() + MessageHeader::SIZE))
    {}

    uint8       getMDUpdateAction() const { return m_Struct.m_MDUpdateAction; }
    int64       getMsgSeqNum()      const { return m_Struct.m_MsgSeqNum; }
    std::string getSecurityID()     const { return std::string(m_Struct.m_SecurityID, 12); }
    Decimal     getMDEntryPx()      const { return m_Struct.m_MDEntryPx; }
    // ...

    virtual void dump(std::ostream & os) const
    {
        m_MessageHeader.dump(os);
        os
        DUMP_FIELD_AS_INT(MDUpdateAction)
        DUMP_FIELD(MsgSeqNum)
        DUMP_FIELD(SecurityID)
        DUMP_FIELD(MDEntryPx)
        ;
    }
};

inline std::ostream& operator<<(std::ostream& out, const PostTrade& obj)
{ obj.dump(out); return out; }
```

Caractéristiques imposées (voir [.github/instructions/cpp-messages.instructions.md](.github/instructions/cpp-messages.instructions.md)) :

- typedefs courts : `uint8`, `int16`, `int64`, `Decimal`, `TimestampNano` — **pas de `std::uint*_t`**,
- préfixe `m_` PascalCase sur tous les champs,
- `<Name>Struct` pour la struct binaire packée, `<Name>` pour la classe wrapper,
- accès au binaire via `reinterpret_cast` + référence `const &` (zéro copie),
- getters : `std::string` pour `char[N]`, type direct sinon,
- `dump()` avec macros `DUMP_FIELD` / `DUMP_FIELD_AS_INT`,
- `operator<<` inline.

## Structure du workspace

```
.github/
  prompts/
    pdf-to-cpp-messages.prompt.md    # Prompt — extraction des messages
    pdf-to-cpp-types.prompt.md       # Prompt — extraction d'un type réutilisable (par §/Table)
  instructions/
    cpp-messages.instructions.md     # Conventions C++ auto-appliquées aux .hpp/.cpp
templates/
  Message.hpp.template               # Template message paramétré
  Type.hpp.template                  # Template type paramétré (struct/enum/bitfield/alias)
include/
  messages/
    Common.hpp                       # Typedefs uint8/int64/..., Decimal, TimestampNano, InputData, DUMP_FIELD
    MessageHeader.hpp                # STUB — à remplacer selon la spec utilisateur
    PacketHeader.hpp                 # STUB — à remplacer si protocole avec framing paquet
    MessageIds.hpp                   # Registre central des opcodes
    types/
      Types.hpp                      # Header agrégateur des types
      GeoPosition.hpp                # Exemple de type généré
examples/
  PostTrade.hpp                      # Exemple de sortie attendue (style maison complet)
specs/                               # (à créer) — vos PDF de spec
```

## Comment l'utiliser

### 0. ⚠️ Pré-requis bloquant : `MessageHeader` & `PacketHeader`

Le workflow **demande systématiquement** avant de générer le moindre message :

1. Quel est le **layout du `MessageHeader`** (champs, taille, méthode `dump()`) ?
2. Existe-t-il un **`PacketHeader`** (framing paquet de transport) ? Si oui, son layout.
3. Les types domaine (`Decimal`, `TimestampNano`, `InputData`) correspondent-ils déjà au projet, ou faut-il les adapter dans [include/messages/Common.hpp](include/messages/Common.hpp) ?

Réponds à ces questions **avant** de continuer — sinon les stubs [include/messages/MessageHeader.hpp](include/messages/MessageHeader.hpp) et [include/messages/PacketHeader.hpp](include/messages/PacketHeader.hpp) seront utilisés tels quels (et marqués `TODO`).

### 1. Déposer la spec

Place ton PDF de spécification dans `specs/` (ex: `specs/protocol_v1.pdf`).

> Si Copilot ne peut pas lire le PDF directement, extrais le texte au préalable :
> ```powershell
> pdftotext -layout specs/protocol_v1.pdf specs/protocol_v1.txt
> ```

### 2. (optionnel) Extraire d'abord les types réutilisables

Si la spec définit des **types partagés** (struct composé, enum, bitfield) dans des paragraphes / tables dédiés, génère-les **avant** les messages :

```
/pdf-to-cpp-types
```

Copilot va te demander :
- le **PDF**,
- la **section / table** précise (ex: `§4.3.2`, `Table 7`),
- le **nom du type** (ex: `GeoPosition`),
- la **catégorie** (`struct` / `enum` / `bitfield` / `alias`).

Le type est créé dans `include/messages/types/<Nom>.hpp` et ajouté à `Types.hpp`.

### 3. Lancer la génération des messages

```
/pdf-to-cpp-messages
```

Copilot va :
1. **Demander** le layout de `MessageHeader` et `PacketHeader` (étape 0).
2. Lister les messages détectés → demander confirmation.
3. Générer un `.hpp` par message (struct + classe wrapper + dump + operator<<).
4. Mettre à jour [include/messages/MessageIds.hpp](include/messages/MessageIds.hpp).
5. Signaler les champs ambigus (`// TODO:`).

### 4. Compiler pour valider

```powershell
# Linux / WSL / MinGW
g++ -std=c++17 -Wall -Wextra -Iinclude -c examples/PostTrade.hpp -o NUL

# MSVC
cl /std:c++17 /W4 /Iinclude /c examples/PostTrade.hpp
```

Les `static_assert` échoueront à la compilation si la taille ne correspond pas à la spec.

## Personnaliser les conventions

- [.github/instructions/cpp-messages.instructions.md](.github/instructions/cpp-messages.instructions.md) — règles de codage.
- [include/messages/Common.hpp](include/messages/Common.hpp) — adapte `Decimal`, `TimestampNano`, `InputData` aux types réels du projet.
- [include/messages/MessageHeader.hpp](include/messages/MessageHeader.hpp) — remplace le stub par le vrai `MessageHeader`.
- [include/messages/PacketHeader.hpp](include/messages/PacketHeader.hpp) — idem ou supprimer si non applicable.

## Pré-requis

- VS Code 1.96+
- Extension **GitHub Copilot Chat** activée
- Modèle Claude (Sonnet/Opus) recommandé pour l'analyse PDF longue
- Compilateur C++17 minimum
