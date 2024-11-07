#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define N 64  // Size of the grid

int mapCharToDigit(char c, int *used_chars, int *next_char_index) {
    if (isdigit(c)) {
        return c - '0';  // Direct map for digits
    }
    if (used_chars[(unsigned char)c] != 0) {  // If character is already mapped
        return used_chars[(unsigned char)c];
    }
    used_chars[(unsigned char)c] = (*next_char_index)++;  // Assign new index
    return used_chars[(unsigned char)c];
}

int main() {
    const char *input = "F C 7 W a A r k z $ y G Q v 3 i x 6 X M D e K w u 4 Y 1 s T # m P B E R ? p j c N t V J q Z 8 d g O U S b f 2 h H + 5 o n I 9 L"
                        "V f d m ? M D i w u x T g 4 L b q j v I U 2 O 8 E W k B c h $ Z F N r H o G K Q z X R y s e C P + 9 p 3 # Y n 5 S t 6 7 a 1 J A"
                        "B R U 1 5 g Q Y X E c d 7 t D j f C s W $ h L k w e N V b 3 p z i m q A # J I M T 9 G S u ? n + P 8 o 6 x H v a Z 2 4 O F K y r"
                        "E $ O T v n j 6 m a K p J H 1 V B A 7 9 P z + y S F Q o 2 U D r 3 W w Y h g k C M 4 b c I f L 5 q Z N G d ? e t # s x 8 R u i X"
                        "Z + c x S H J t o R 9 ? C A N F i 1 d n a Q 5 V P M K I g X L G s D $ 8 4 6 u y k W r h U 2 O # E w j z T 7 B m v q f 3 p b e Y"
                        "2 K L 9 h w y # n U B f O P Z r Y H 4 E ? t m g q v d a C 5 6 8 + x b 7 e X 1 S 3 F i j Q A p o c k W s R J I u T G N z M D V $"
                        "8 b z I u G P N M q s e 5 # S + J R Z F p 3 r o i y n 9 A x O f v L T V t 2 U a 6 m E w H Y g 7 X D 4 Q C 1 $ K d j ? k B W c h"
                        "4 e s p q 3 o X 8 I 2 k h 6 W Y b N S G c # u T t + J R H ? j 7 n f Z d O z 5 9 1 K v $ B x a D V i L A F y r M Q m P g w E C U"
                        "e p v j 4 $ z c A m # u T C n w I X x P o K W 2 a 9 3 O 6 Q 7 q U R ? k L B t g 8 D 5 H b J N Y h F 1 E S + s G y f d i Z M r V"
                        "J Y E V 2 d ? s i D j 5 P r I 1 + g z Z M A 4 $ F H x 8 k e B w q Q y W T f 9 # K G p X h v o c 7 U m O n u N b C R S 6 t 3 L a"
                        "G y t U H r 5 a 7 6 S g e 8 ? h c s R J F j w L d i V Y 4 f z K O + C M A D Z 1 u x m k # E P T $ 3 Q o W X 9 B N b v q I p n 2"
                        "M x i D g R q Q 3 V J E 2 f 9 o 1 v B t r ? 6 H c $ Z # h S U A 4 e 7 p b a d N L + F C O y I n k 5 K Y P T 8 z W u X s m G w j"
                        "S n u k f L N A p s t $ F c R X 5 Q 9 h # b y q C g m J M D ? + G j o i I V E K e 7 U r 2 6 3 W a 4 v x Z d H w Y P 8 1 T O z B"
                        "b 9 o O P C I B a x z H Z y k M 7 p e U f u Y i G j r T 1 N W R 8 3 S s X n v m w $ Q ? V d 4 q A L # g 6 D t 2 E F K c + 5 h J"
                        "m W + F K 8 # w d G q N b B Y O n D 3 T C S a E 5 P L X o p v I z 6 J c r u h 2 9 Z f t A g 1 s y M ? i e V R j U Q 7 4 k $ H x"
                        "T 7 Z 6 1 h X 3 K + L U 4 W v Q V d O N G k 8 m 2 n t y u b E s $ P 5 x Y H w F B a j z S M i R I f r p c q J C D g 9 ? o A # e"
                        "f r J S 9 s $ z 6 e + V L 2 5 B 3 4 A Q W y c v 8 C # ? K E H 1 N 7 u I P Z X q G R o U d O Y b t x a F j M w n m i g p D h k T"
                        "3 F n t O 1 m D W o i j d Z E v 8 I k Y u p 2 6 f R A x z y M g V $ + U C Q S T # P e s N c 5 K J h X 9 q B L 7 a H b G r w ? 4"
                        "k w # u U x 4 I 1 n X P s G h N Z r ? R B V M d 9 c 2 q a W t O H v K F j e y L D A T 6 3 $ 7 J f + g C m p b i 5 Y E Q S 8 o z"
                        "H T b Q 7 W Y o S F k t I w z c K G + g 1 X N s 6 U v e $ P n D h 8 d 2 9 4 r ? V i C x p a j m # A E 5 3 Z O R L B J M f q u y"
                        "A g 8 N j B L y b p 3 # R u a D e T n o x 7 C U k J h r d i G 5 c Y z f M O m E H v S 4 w q Q 2 1 6 P $ V K ? s I X Z + 9 t W F"
                        "5 X K P c V a h y 8 H 7 m O M U w E L z q i J f B b + j N s o 4 R A k g 1 t G p r u ? n F 9 Z I T Q 2 d Y W D S $ x 3 C # e v 6"
                        "p ? M v e Z 6 E r f g Q q K $ C m O t S b a # 9 V L I 3 T F Y X D J B n x 5 i w W 1 + 8 y h k z N c G U 4 o u H A d 2 j 7 s P R"
                        "R q C i + 2 d G J 4 ? A Y 9 T x j $ h H 5 P D F Q Z w m 7 u S p # s a b 3 o W 6 E B L f X t M g 8 r z v I k y e O c n K V N U 1"
                        "9 B j X M p C R U b E 4 G S s 2 W V q O K Y o h D 5 T g m J x # a F L v 7 8 z P I r H d t 3 + i ? 1 w e Q n c A f y $ u N k 6 Z"
                        "W 4 e G E N V S B J 6 y w p t q P 3 H u X d A D + I M $ O n 2 k m # h C c ? R Z f j K 1 a L z x s b i T 7 g U v o 9 r F 8 Y 5 Q"
                        "w A T z 8 K t F O i m D M h C ? E L y e + G B R 3 f U d 9 v Q o Y k 6 $ s q x 4 b J 2 # c p u V j a 5 W N r X Z n I H S g 7 1 P"
                        "r i f 2 L + x q N # u 1 9 g c k t 5 m w Q s U C y X ? E j G P S T H p O W b n 3 h Y Z 7 v 8 e 4 B d I K $ F o 6 R J a A z V D M"
                        "U D P y 6 Q Z 5 F d r I 3 a 7 T $ M f 2 n c z p H B s t L 1 K i e 9 V J u A N X ? C W o g R m S Y # q h 8 4 G k b w + v j x E O"
                        "v O m c b Y g J R X Q x H n 8 z 6 Z N 1 j 9 F a W w 4 7 e q C u 2 E I r f K + o A 5 D M $ s T k p y 3 L t P S V h # i U ? B G d"
                        "u # k ? o a 1 H f j 5 v V L K $ S 8 b r i g I 7 R A 6 F Y Z N h d U Q w B y D t X n q P 9 G E O 2 J M m z C x + 3 e T W 4 c s p"
                        "7 d h s $ I 3 n e W Y o A + P Z # ? J v k x T 4 z V p b 8 c r a M 1 i S G j g 5 U y B Q 6 w F N 9 u f D H R E O q L t X C 2 m K"
                        "O P X Z p T H 1 # 7 M Y D e + n G K r i 6 f g c s k 8 u 3 L V ? o b 4 t y F A R J w I q C N d v z j x B E U 5 $ 2 h Q m W a S 9"
                        "h k A Y y f M v $ P 8 s K J m E d + j B Z W Q t # 1 i D w a 4 F 7 z 9 L N r 2 V p o O e x u c ? b g n q G S T I X C U R 6 H 3 5"
                        "i Q r B z j n ? L g v 3 k d U 6 u 7 8 $ e w 1 A o q 9 H + I T b S 5 m Z a # Y J P s h D K F 2 y R C t M X O p W c E V x G 4 N f"
                        "K V R 5 N b e 7 x z w i ? T q A a m o # H v S X J p c P t 2 Z E Q G 8 B d U C W 4 3 6 Y k n f 9 F s h + r L 1 D j $ M I u y O g"
                        "+ c I E F S G g u y f 9 r b p 5 M n 2 C V L k J U 6 e K X z R x 1 h D j H P 3 O a Q 7 i m W $ A o Y 8 ? v N # 4 t Z s T q d B w"
                        "d 6 W J w # u L I Q F O S V X t z U D x E 4 P N $ r j C 5 A h M g T s + q v c f Z H 1 G R b B 8 K 7 9 y 2 a m 3 e n o Y i ? p k"
                        "C a x o s m 2 8 4 h G Z j R B W y 9 5 ? I q 3 Y v N 7 Q f O g n w p X K i k $ e t U M V + T S E u H c J A 6 P d z D L r 1 # F b"
                        "D U 4 3 t q 9 $ C H o c N 1 2 a T F p s R O h b Y G W S B m y d x I M ? n E 6 u 5 L # g j z X r e V Z w k i f Q K 8 A J P v 7 +"
                        "6 H 2 A m c 8 K s k C q $ I d u 9 y Q 7 z 1 n P 4 Y F i Z # + t j X v T 5 x e h S E g B f V U a 3 N R b o w W r G ? p L O J M D"
                        "N o $ b x y i O D M R L + 7 J S ? c W d t C Z u j h X G Q r 9 3 p a 2 # V w F 8 n 6 k 5 4 P H 1 v z s f K E Y q B U I e A g T m"
                        "t j g 4 3 E F d Z c p h v Q V G H x U A T I f B O S b W n o m N L i P D 6 Y J 7 C z w 2 ? K R e M X u a 5 # k y + r 1 $ s 9 8 q"
                        "Q L 1 a V D + M ? 2 A r i o 6 g F J w k v R E j p u z s I H 8 T t S c G K N B d q O X Z 7 # y $ x m e P U h C 9 4 3 Y f 5 n b W"
                        "P I Y n k 9 R r T B 4 K f U j m N b M + L 6 G 5 1 d D v V C q e E u A y $ 3 Q H x h s p i o W F O ? S Z J 2 g 8 w 7 # a X z t c"
                        "q 5 ? f G J S p E 9 N n z 3 e # X Y K m g r $ O A x a c U R w y W o 1 4 Z + b s j M 8 L D Q v t 6 T B V i I 7 F u k h 2 H P d C"
                        "s Z w e C U v u 5 1 W b X Y O P h a # D 2 8 V S 7 E B f ? K J $ I n g q z m M k c T 9 + G r A 3 H t d 4 L Q j p 6 o R N y F x i"
                        "# z B h X 7 T W t w a F 8 x y H 4 e i p s o q 3 g 2 5 L P M k 6 ? O U 9 R C f r m N d b Y I J u G n + 1 D $ A c V v j Z E Q K S"
                        "g 1 D H Z i B j Q 3 P S # m A L p q Y f h J s n b O o U E 9 I C 5 ? e N k W a G 2 d z F r X K w 4 v $ u y t + T 7 M c V x 6 R 8"
                        "L 3 6 w I ? O e q 5 T W U $ G R 2 t g X 9 F b r h m 1 + S Y A B K d f u Q 7 o n v k c N Z D x M i p C 8 s z V E P 4 y H J j a #"
                        "Y t Q q R u s 2 k N d C c X b f U z V y S D e + n ? G 4 i g 5 H r w O 6 v I p x 7 8 a A o B 9 L m K J # M j h P 1 W F E $ T Z 3"
                        "$ M V 7 J P c 4 H t h 8 u i g I R B C 6 3 N ? K e T q k W j a 2 A Z Y E U s L D + f y O 1 m # p n o F r w x d X 9 5 z b v S Q G"
                        "a h y # d v b T + r D 6 B j x K L P E 5 w M 7 1 X 3 R z J V c Q 9 t F m 8 S H i s q $ I W 4 ? G U e k N g A Z f p O u n 2 C Y o"
                        "X G S C r z K U 2 v O M p s o J k u a c d T x # N 8 y Z D 7 F P B 4 3 1 + $ V j Y b n E e i t Q W I H R 9 5 6 L g A m w h f q ?"
                        "n m 9 8 W F k x V Y e z E ? w y v o I 4 A $ H Z M K u p # 6 f L J C R X 2 c P b g S 3 T 5 j h U Q q D 7 O G a 1 r N B t d i + s"
                        "o E N + A 5 p f 9 Z 1 a n F 4 7 Q i G 8 O m j W x s $ w r t d v y M # h g T q z R V J u P H 6 C S 2 Y c B b 3 ? k K e D L U X I"
                        "y s a R Y 6 f 9 c K I B t D # p O h $ 3 m Z d G L 7 H n q w b J u V N 5 F 1 ? U Q 2 x W z S r j C E A X + 8 i g M T k P e o 4 v"
                        "I u 3 M # O 7 b G S Z 2 1 E i e g f F q J B t Q m D P 6 v d X U k K j z p R s A o ? N 9 T + w h L W y n a c 4 x 8 V C 5 Y r $ H"
                        "x S G L Q 4 A + h T b w W N F s C 2 6 j 7 H 9 M K t g 5 R 8 e Y Z r n 3 J i # v d I P a E U q B D $ V k 1 m z o ? p O y c X f u"
                        "j 2 H r T X U V v ? $ m y 5 u 3 o W P b Y + p z Z # S N G k i 9 6 g t a E M 8 B F c 4 K n C s f w R O I h e q J x 1 D d Q L A 7"
                        "? N F d n k W C Y L V J o z H 9 s # c K 8 5 X x T a O 2 y 4 3 j f q G e m h 7 I $ p A R M 1 D Z r P 6 t u v Q U i S w B b + g E"
                        "1 J q g B e w P j O U X x k f d A S T a N E v ? r o C M F $ u c b y H Q D L 4 + i # t m 8 5 V 6 Z G 7 2 p 9 K Y s z W h 3 R I n"
                        "z 8 5 $ i o h m g C n + 6 M r 4 D k 1 V y U R I ? Q E A p B s W X c x P w 9 T Y O e u v L 7 G H d S b j f 3 F N J a q # K Z 2 t"
                        "c v p K D t E Z P A 7 R a q Q 8 r w u L 4 n i e I z f h x + 1 V C 2 W o S d O $ y g Y 3 J k b X 5 B T H ? s M # F 6 G 9 U m j N";
    int used_chars[256] = {0};  // Map of characters to unique indices
    int next_char_index = 10;  // Starting from 10 for non-numeric characters

    // Start printing the array as a 64x64 grid
    printf("int fixed_board[%d][%d] = {\n", N, N);

    int count = 0;  // Counter to track the number of characters per row
    printf("\t{");  // Start the first row
    for (size_t i = 0; i < strlen(input); ++i) {
        char c = input[i];
        
        // Skip spaces
        if (c == ' ') {
            continue;
        }
        
        int mapped_digit = mapCharToDigit(c, used_chars, &next_char_index);
        
        // Print the mapped digit, with a comma if it's not the last in the row
        printf("%d", mapped_digit);
        
        count++;
        
        if (count % N == 0) {  // End of a row
            printf("}");
            if (i != strlen(input) - 1) {
                printf(",\n\t{");  // Start a new row if not the last row
            }
        } else {
            printf(", ");
        }
    }
    
    // Ensure the last row ends correctly
    if (count % N != 0) {
        printf("}\n");
    }

    printf("\n};\n");

    return 0;
}
