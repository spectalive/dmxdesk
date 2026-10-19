Rediseño UX/UI de dmxdesk — Vibra Lighting Console

Revisión de las 8 pantallas · Tablet 1024 × 600 · Consola táctil para directo

1. Cambio de enfoque

La aplicación actual tiene una base funcional bastante buena: es oscura, los controles son grandes, hay pocas distracciones y las acciones están agrupadas. Pero todavía se parece demasiado a una representación táctil de los controles internos de QLC+.

Mi propuesta es convertirla en una superficie de directo, donde lo más importante no sea qué botones existen, sino qué está haciendo la iluminación ahora mismo y qué va a pasar cuando se pulse algo.

La interfaz debe tener dos personalidades dentro del mismo sistema: SHOW es una mesa de actuación inmediata, para alguien que necesita reaccionar sin pensar; COLOR, CABEZAS, GOBOS, PRISMA y las demás páginas son superficies de selección para quien prepara o ajusta el espectáculo. Ambas deben compartir los mismos estados visuales, tamaños, criterios de activación y mecanismos de seguridad.

El criterio de éxito es sencillo: alguien que no sabe qué es DMX debe poder poner la sala en modo CHARLA, identificar que está activo, lanzar un efecto momentáneo y recuperar AUTO, todo sin instrucciones.

2. Problemas

Ordenados por impacto real durante un evento, no por estética.

P0 — Problemas que afectan al directo

01. SHOW no tiene un protagonista visual claro.

En show.png, AUTO, CHARLA, TRANQUILO, FIESTA, LOCURA, BLANCO TOTAL y TODO NEGRO aparecen como siete tarjetas casi equivalentes. Sin embargo, son las decisiones más importantes de toda la aplicación: determinan cómo queda la sala.

El usuario tiene que leer las siete opciones en lugar de reconocerlas inmediatamente.

Solución: convertirlas en un bloque principal de escenas con una jerarquía muy clara, nombres grandes y selección persistente. AUTO puede ser la primera opción, pero debe destacar especialmente cuando está funcionando.

02. No se distingue suficientemente lo activo de lo disponible.

En show.png, los botones de escena tienen un fondo similar. En color.png, los programas automáticos tampoco muestran visualmente cuál está ejecutándose. En cabezas.png y gobos.png, la diferencia entre una elección activa y otra sin activar no resulta evidente.

Una consola de iluminación no puede limitarse a confirmar que se ha tocado un botón: tiene que mostrar el estado que realmente está reproduciendo.

Solución: cualquier selección activa tendrá una combinación de borde, fondo, indicador y texto. No dependerá exclusivamente del color. El estado deberá proceder de QLC+ cuando esté disponible; si solo se conoce el último comando enviado, no se presentará como una confirmación del estado real.

03. Los controles momentáneos parecen botones normales.

En show.png, FLASH, FLASH LENTO, FLASH COLOR, STROBO, STROBO SUAVE, HUMO YA y HUMO VERT utilizan tarjetas con aspecto similar a las de selección. La indicación «pulsa · máx 8 s» está demasiado abajo y en gris.

El usuario puede interpretar que un toque inicia un efecto persistente, o mantenerlo pulsado sin entender cuánto tiempo queda.

Solución: presentar estos controles como pads momentáneos identificados con «MANTENER». Durante la pulsación, cambiarán de estado y mostrarán el tiempo restante hasta el límite.

04. PARAR TODO no explica lo que ocurrirá después.

El bloque rojo de la derecha es fácil de encontrar, lo cual está bien. Pero «1.0 s fade» no comunica a un operador no técnico si detendrá secuencias, apagará luces, cortará el humo o simplemente oscurecerá el rig.

Hay que conservar el nombre PARAR TODO, pero hacer explícito su efecto real, de acuerdo con el control del show map. Tampoco debe confundirse con TODO NEGRO, que es una escena normal.

Solución: mantener PARAR TODO fijo en la esquina inferior derecha, con información veraz sobre su acción. Su estado visual durante la activación debe ser diferente del de una escena seleccionada.

05. El fader MASTER está visualmente incompleto.

En todas las capturas, la derecha muestra MASTER, 100%, una pequeña barra blanca horizontal y un rectángulo grande y vacío.

No parece un regulador de intensidad que pueda tocarse y arrastrarse. Además, un usuario que ve 100% no sabe con rapidez dónde está el recorrido útil.

Solución: dibujar un fader vertical real dentro del mismo carril, con pista, nivel de relleno, tirador grande y porcentaje visible. Al moverlo, la respuesta visual debe ser inmediata.

06. No hay una política visible de pérdida de enlace.

En la barra superior aparece «enlazado», pero es pequeño, gris y separado del resto de indicadores.

En esta aplicación, perder la conexión con QLC+ no es un detalle técnico: significa que el operador puede estar enviando órdenes que nunca llegan.

Solución: un indicador de conexión reconocible con texto y estado. Cuando no haya enlace, mostrar «SIN CONEXIÓN» y deshabilitar los controles remotos con una razón legible. PARAR TODO debe seguir visible, pero nunca aparentar que ha detenido el sistema si no pudo transmitir el comando.

P1 — Problemas que retrasan las operaciones habituales

07. SHOW ocupa prácticamente toda la pantalla, pero no organiza bien la urgencia.

En show.png, las escenas, flashes, humo, ambiente, colores y tempo se colocan en franjas horizontales muy compactas. La sección «COLOR DEL RIG» queda especialmente comprimida abajo.

Hay mucha información, pero no existe una separación suficientemente fuerte entre tres intenciones diferentes: cambiar la escena, lanzar un efecto y realizar un ajuste.

Solución: distribuir SHOW en zonas estables. Arriba, las escenas; en el centro, los efectos momentáneos y el humo; abajo, los ajustes rápidos de color y tempo.

08. Las etiquetas pequeñas son demasiado pequeñas para un local oscuro.

«LA SALA ESTÁ ASÍ», «GOLPES: MIENTRAS PULSES», «HUMO A MANO» y «AMBIENTE: DISPARA AL ELEGIR · OFF LO PARA» necesitan leerse para comprender el funcionamiento, pero parecen etiquetas administrativas.

Lo mismo sucede con los «500 ms», «20 funciones» y «23 funciones» en SPEED.

Solución: tamaños mínimos más consistentes, textos operativos de mayor contraste y eliminación de información técnica de baja prioridad cuando no ayuda a tomar una decisión.

09. La zona de humo contiene dos modelos de interacción poco diferenciados.

En show.png, HUMO YA y HUMO VERT son acciones momentáneas, mientras que OFF y «cada 1 min», «cada 2 min», «cada 4 min», «cada 8 min» configuran un ambiente periódico.

Están juntos, pero no lo bastante diferenciados por comportamiento. Eso es especialmente problemático cuando alguien tiene prisa o no conoce el sistema.

Solución: separar visualmente «Humo manual · mantener» de «Humo automático · seleccionar». Mostrar qué intervalo está activo y cómo detenerlo mediante OFF.

10. Los nombres de color se truncan en SHOW.

En la última fila de show.png vemos «Rojo Fu...», «Verde ...» y «Azul Ci...». Son precisamente los casos donde el matiz importa.

Solución: las muestras deben tener un círculo cromático protagonista y un nombre completo distribuido en dos líneas cuando haga falta. No debe utilizarse truncamiento para diferenciar colores disponibles.

11. COLOR desperdicia espacio y mezcla dos formas de control sin dejar clara la prioridad.

En color.png, las seis tarjetas de programas están arriba y doce colores debajo. La zona inferior derecha se encuentra casi vacía mientras la navegación de bancos queda separada en grandes rectángulos al pie.

También hay dos niveles de automatización que pueden confundirse: AUTO como sección y programas como «Colores completos» o «Colores simples».

Solución: programas automáticos en una banda superior compacta y colores fijos en una cuadrícula regular, dando prioridad a identificar y confirmar el color elegido.

12. Las páginas PIXELES, CABEZAS, GOBOS y PRISMA utilizan prácticamente la misma representación para cosas que funcionan de forma distinta.

En pixeles.png, Effect 1 y Effect 42 comparten un icono genérico. En cabezas.png, Círculo y Diamante tienen el mismo icono de movimiento. En gobos.png, Gobo 1 y Gobo 17 son visualmente indistinguibles. En prisma.png, los valores 1, 2, 3 y 4 se parecen demasiado a las acciones de giro.

Solución: mantener una misma estructura de selección, pero distinguir las categorías con iconos semánticos cuando realmente ayuden. Cuando QLC+ solo proporcione un número sin una representación fiable, mantener el número grande en vez de inventar un dibujo.

13. Los bancos parecen parte del contenido y no de la navegación.

En color.png, cabezas.png y gobos.png, las grandes tarjetas inferiores «AUTO · ELEGIR 1/2», «ELEGIR 2/2» y «GOLPES» ocupan más altura de la necesaria.

Además, esta navegación desaparece o cambia de posición en otras páginas.

Solución: una barra de bancos compacta, colocada siempre en la misma zona de las páginas que la necesiten, con selección inequívoca y una etiqueta que explique qué contenido se está viendo.

14. PRISMA deja gran parte de la pantalla vacía.

En prisma.png, nueve botones se concentran en la zona superior izquierda. La mitad inferior queda sin función.

No hace falta llenar cada píxel, pero los controles existentes podrían aprovechar mejor el tamaño disponible.

Solución: cuadrícula adaptada al número de elementos, aumentando la altura útil de las tarjetas cuando haya pocos controles. Sin añadir controles ficticios para llenar espacio.

15. CONTROL parece una página inacabada.

En control.png, cuatro barridos ocupan una fila, y «Luz del humo vertical» aparece sola debajo. El resto de la pantalla está vacío.

Además, las cuatro acciones tienen el mismo tratamiento visual pese a sus diferencias de dirección y comportamiento.

Solución: disponer los barridos como cuatro acciones grandes en una matriz 2×2 y reservar un bloque propio para la luz del humo. Es una página de acceso esporádico; puede permitirse botones más grandes.

16. SPEED duplica demasiada superficie sin aprovechar la diferencia funcional.

En speed.png, «Tempo Show» y «Movimiento» tienen paneles de idéntico tamaño, pero los valores de BPM aparecen desconectados del resultado en milisegundos y los controles pequeños están colocados entre varios espacios vacíos.

«Más lento x2» y «Más rápido x2» no muestran suficientemente bien el efecto de la operación.

Solución: dos módulos de tempo perfectamente paralelos, cada uno con un BPM dominante, TAP grande y ajustes agrupados. «TAP ambos» debe ocupar una posición estable y suficientemente visible.

P2 — Acabado y coherencia

17. Falta consistencia entre la escala de los controles. Las tarjetas de SHOW, las de COLOR y las del resto de páginas tienen alturas y densidades de contenido diferentes sin una justificación operativa clara. Definir tres componentes principales: escena, acción momentánea y selector.

18. El idioma visual está a medio camino entre software técnico y lenguaje cotidiano. En SHOW aparecen CHARLA, FIESTA y LOCURA, mientras en otras pantallas aparecen Effect 1, Insert Prism o «Tiempo x1». Los nombres proceden del show map y deben conservarse; la interfaz puede aportar contexto sin renombrarlos.

19. La barra superior compite con el contenido pero informa poco. Ocho pestañas, batería, Wi-Fi, enlace, ajustes y bloqueo están concentrados en 40 píxeles. La navegación sigue siendo necesaria, pero sus estados deben tener más contraste.

20. Los iconos no aportan suficiente información. La repetición de pequeños símbolos en GOBOS, PIXELES y CABEZAS consume espacio sin ayudar a elegir. Los iconos deben distinguir acciones; si no pueden hacerlo, el texto debe dominar.

21. No se muestra una diferencia suficientemente clara entre deshabilitado y simplemente inactivo. Especialmente importante para los controles que el QLC+ real no pueda ejecutar: una tarjeta gris no explica por qué no funciona.

22. El bloqueo no tiene suficiente presencia contextual. La barra muestra el candado, pero no se ve cómo quedaría la superficie bloqueada. Un bloqueo eficaz debe impedir toques accidentales y comunicar de forma evidente cómo mantener un segundo para desbloquear.

3. Propuesta
3.1. Una consola de directo, no un panel de administración

La arquitectura propuesta conserva las ocho pestañas, la barra superior y el carril derecho, pero reorganiza los contenidos según lo que el operador necesita hacer.

ESTRUCTURA BASE · 1024 × 600

SHOW　 COLOR　 PIXELES　 CABEZAS　 GOBOS　 PRISMA　 CONTROL　 SPEED

ZONA PRINCIPAL

Escenas o selectores

ZONA SECUNDARIA

Efectos, ajustes y bancos

MASTER

100%

PARAR

TODO

Esquema de jerarquía, no representación a escala. El carril derecho real tendrá 148 px.
3.2. Sistema visual implementable en C

La dirección artística es una consola profesional contemporánea: superficies en grafito, controles sólidos, contraste elevado, bordes discretos y color reservado para acciones y estados importantes.

Nada de degradados, transparencias complejas, imágenes, sombras o animaciones decorativas.

Tokens de color

Fondo principal

Background general

#0C0E12

Superficie

Tarjetas y paneles

#1B1F27

Superficie elevada

Botones de interacción

#262C36

Borde

Separadores y contornos

#39404D

Texto principal

Acciones y valores

#F5F7FA

Texto secundario

Instrucciones

#ADB6C4

Acento

Foco y selección

#8198FF

Activo

Fondo de estado activo

#24385B

Confirmación

Enlace y funcionamiento confirmado

#3EE0AA

Deshabilitado

Estado no disponible

#626B79

Peligro

PARAR TODO

#C92F2F

El acento azul-violeta se utiliza para controles seleccionados. El verde indica un estado operativo confirmado, no un segundo sistema de selección. El rojo se reserva para la parada global y avisos verdaderamente críticos.

Los colores de las luminarias no utilizan estos tokens: sus muestras representan el color real correspondiente.

Retícula y dimensiones

Elemento

	

Especificación




Pantalla

	

1024 × 600 px, sin scroll




Barra superior

	

48 px




Carril derecho

	

148 px




Área principal

	

844 px útiles aprox.




Margen general

	

16 px




Separación entre tarjetas

	

8 px




Separación entre secciones

	

12–16 px




Radio de botones

	

12 px




Radio de paneles

	

16 px




Altura táctil mínima

	

48 px




Botones principales

	

64–90 px de alto




Bordes

	

1 px; 2 px en selección

Tipografía Inter

Uso

	

Tamaño / peso




Valor principal BPM / MASTER

	

34 px / 700




Nombre de escena principal

	

17 px / 700




Acción / programa

	

14 px / 600




Etiqueta de grupo

	

12 px / 700




Texto secundario

	

12 px / 500




Información auxiliar

	

11 px / 500

Las tarjetas de selección pueden utilizar texto en dos líneas, nunca truncar nombres importantes. «Diamante Simultaneo» debe verse completo, aunque haya que reducir a 12 px dentro del mismo componente.

3.3. Comportamientos y estados

El sistema debe distinguir seis situaciones, no simplemente botón pulsado y botón sin pulsar.

Disponible

Acción lista

Activo

Selección actual

Manteniendo

Efecto momentáneo

Deshabilitado

Con motivo

Sin conexión

Sin respuesta

Parada

Acción crítica

Reglas de interacción:

Selección exclusiva: cambiar entre escenas o entre elementos de una categoría, cuando el show map establezca exclusividad. El elemento anterior pierde la selección.

Pulsación momentánea: FLASH y HUMO YA actúan desde touch-down hasta touch-up o hasta su tiempo máximo. Mostrar el estado «MANTENIENDO» y una barra de duración. También cancelar al bloquear o perder el enlace; la gestión segura del cese de la orden en QLC+ requiere verificación.

Deshabilitado: el botón conserva su posición y nombre, pero explica «No disponible en este show» o el motivo real correspondiente.

Cambio de página: no detiene efectos persistentes, salvo que el comportamiento real del show map establezca lo contrario.

Bloqueo: superposición estática «MESA BLOQUEADA · Mantén el candado 1 s», sin desplazamientos ni gestos de precisión.

Fader: respuesta táctil inmediata, ajuste vertical y zona de agarre amplia. El comportamiento de toma de control debe evitar saltos inesperados si la posición visual difiere de la intensidad real.

3.4. SHOW: tres horizontes de uso

La nueva pantalla SHOW se distribuye así:

Estado de la sala — el bloque más importante y más cercano al inicio de la pantalla.

Acción inmediata — flashes, estrobos y humo manual, visualmente distinguibles por exigir mantener.

Ajustes de ambiente — humo programado, colores del rig y BPM.

Los siete estados de sala tienen prioridad sobre el resto, pero no se ocultan los controles existentes. Se emplean tarjetas más compactas para lo secundario y una distribución sin desplazamiento vertical.

La barra derecha permanece idéntica al cambiar de pestaña.

3.5. Páginas de selección: un solo patrón, diferentes datos

COLOR, PIXELES, CABEZAS, GOBOS y PRISMA comparten un componente base:

Franja de programas AUTO, con los nombres exactos del show map.

Cuadrícula de selección, con botón activo reconocible.

Navegación de bancos donde exista.

Estados de QLC+ aplicados a cada elemento.

Las cuadrículas no deben forzarse a seis columnas cuando no convenga. COLOR utiliza muestras cromáticas más grandes; CABEZAS admite nombres de dos líneas; GOBOS da prioridad al identificador exacto; PRISMA puede emplear botones más amplios.

3.6. SPEED: control táctil pensado para el ritmo

El diseño actual puede mejorarse sin alterar su funcionalidad.

Los dos módulos —Tempo Show y Movimiento— tendrán la misma disposición, con un número BPM de gran tamaño, un TAP dominante, los controles de ±1 BPM próximos y los multiplicadores juntos.

TAP ambos será una acción independiente debajo de los dos módulos, no un elemento suelto.

Nuevo, opcional: añadir una señal visual de pulso basada en el BPM ya conocido, sin modificar el motor ni su reloj. Solo tendría sentido si puede dibujarse sin afectar a la capacidad de respuesta táctil.

3.7. Priorización de desarrollo

Prioridad

	

Cambio

	

Coste estimado




P0

	

Estados activos fiables y consistentes

	

UI + integración de estado




P0

	

Reorganización de SHOW

	

UI




P0

	

Pulsación momentánea, duración y liberación segura

	

UI + lógica táctil




P0

	

Fader MASTER reconocible y usable

	

UI + lógica de fader




P0

	

Enlace caído y bloqueos de acciones

	

UI + estado de conexión




P1

	

Cuadrículas y bancos unificados

	

UI




P1

	

SPEED legible y compacto

	

UI




P1

	

Etiquetas completas, contraste y tipografía

	

UI




P2

	

Indicador visual de pulso — nuevo

	

UI

El objetivo no es introducir un sistema nuevo de escenas, grabación, programación o efectos. Todo debe seguir conectado a los controles existentes del show.

4. Referencias y patrones adaptados

He contrastado cuatro productos que llevan años resolviendo problemas similares. Ninguno es una plantilla para copiar: son referencias de interacción.

1. ChamSys MagicQ

Manual oficial · Execute Window

Patrón consolidado: superficie Execute, donde grupos, paletas, macros, cues, secuencias, botones y faders pueden organizarse como una cuadrícula dedicada a la reproducción, incluso a pantalla completa.

Caso difícil que resuelve: un espectáculo complejo puede necesitar un entorno de programación muy potente, pero el operador durante el directo solo necesita un conjunto reducido de acciones. MagicQ permite separar ambas interfaces. También admite diferentes tamaños de controles, estados y funcionamiento momentáneo de botones. 
ChamSys
+1

Adaptación a dmxdesk: exactamente esa separación, pero con una estructura estable en vez de un editor de cuadrículas. SHOW será el equivalente a una superficie Execute preparada para una persona sin formación.

No copiar: el editor de páginas, matrices configurables, comandos especiales y herramientas de programación. Son innecesarios para este tablet.

2. Avolites Titan

Manual oficial · Titan Mobile

Patrón consolidado: zonas físicas diferenciadas para playback, selección de páginas, atributos y funciones especiales. La consola mantiene una relación muy clara entre control y tarea.

Caso difícil que resuelve: ajustar atributos de las luminarias mientras continúan ejecutándose otras secuencias. El operador necesita entender qué está manipulando y evitar cambiar accidentalmente el conjunto equivocado. Los playbacks de Titan también permiten comportamientos de activación momentánea y mantenida. 
Avolites Manual
+1

Adaptación a dmxdesk: áreas persistentes, especialmente MASTER y PARAR TODO, más una separación entre escenas, efectos momentáneos y atributos del rig.

No copiar: ruedas de atributos, teclado numérico, comandos de programación y organización física de una mesa completa. Sin entrenamiento, aumentarían la carga cognitiva.

3. Lightkey

Página oficial · Funciones de Live Pages y cues

Patrón consolidado: páginas Live con botones grandes y faders, presets accesibles por acción y controles pensados para pantallas táctiles. Los grupos de cues admiten selección exclusiva y comportamientos de toggle o flash.

Caso difícil que resuelve: mantener un espectáculo en funcionamiento mientras alguien modifica manualmente un único elemento sin tener que abrir todo el entorno de programación. Lightkey también proporciona sincronización por pulsaciones TAP y modificadores de velocidad. 
Professional DMX lighting control. Mac style.
+1

Adaptación a dmxdesk: tarjetas grandes, selección exclusiva visible, acciones momentáneas y controles de tempo simples.

No copiar: gestos multitáctiles, visualizador de luminarias, edición profunda de presets o jerarquías anidadas. Requieren más potencia gráfica o precisión de la necesaria.

4. grandMA3

Documentación oficial · grandMA3 onPC

Patrón de referencia: superficies de playback con ejecutores, páginas, controles persistentes y diferenciación entre edición y ejecución.

Caso difícil que aborda: manejar muchas capas de reproducción sin perder el contexto de qué está bajo control. Es una necesidad típica de grandes espectáculos, aunque su complejidad excede ampliamente la de Vibra.

Adaptación a dmxdesk: utilizar la disciplina de la consola —zonas fijas, prioridades claras y consistencia de estados—, pero eliminar por completo la complejidad que solo entiende un iluminador profesional.

No copiar: múltiples ventanas, comandos, jerarquías de playback, configuración libre de ejecutores o interfaces repletas de valores DMX.

Conclusión de las referencias

La idea más valiosa que comparten es que la superficie desde la que se ejecuta el show no debería parecerse a la superficie donde se programa.

dmxdesk debe ser incluso más sencilla: QLC+ ya hace el trabajo complejo en el Mac. La tablet no necesita replicarlo; necesita ofrecer un mando extraordinariamente claro sobre lo que ya existe.

5. Mockups

Las tres maquetas son propuestas conceptuales de la misma consola, no pantallas independientes. Comparten la barra superior, el sistema de colores, los estados de selección, el fader MASTER y PARAR TODO.

En SHOW quiero que predomine el control inmediato, con escenas reconocibles de un vistazo. En COLOR, la distinción entre programas y colores fijos. En SPEED, los dos ritmos deben poder ajustarse sin buscar los botones.

Las selecciones activas representadas son ejemplos visuales, no lecturas reales del estado de QLC+.

Conceptual mockup — SHOW
1024 × 600

Conceptual mockup — COLOR
1024 × 600

Conceptual mockup — SPEED
1024 × 600

Las imágenes son referencias visuales conceptuales; las especificaciones de texto, tamaños, estados y comportamiento anteriores son las que deben guiar la implementación. Las dos últimas se han separado de la composición generada y conservan márgenes para mantener su proporción.
¿Te ha resultado útil esta conversación hasta ahora?
