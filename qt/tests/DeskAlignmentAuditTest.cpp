#include <QGuiApplication>
#include <QQuickItem>

#include "../desk_alignment_audit.h"

int main(int argc, char **argv) {
    QGuiApplication app(argc, argv);
    QQuickItem root;
    root.setWidth(1024);
    root.setHeight(600);

    QQuickItem tempo_title(&root);
    tempo_title.setX(552);
    tempo_title.setWidth(60);
    tempo_title.setObjectName("align-left:direct-tempo");
    QQuickItem tempo_number(&root);
    tempo_number.setX(552);
    tempo_number.setWidth(82);
    tempo_number.setObjectName("align-left:direct-tempo;align-center:direct-tempo-bpm");
    QQuickItem tempo_base(&root);
    tempo_base.setX(552);
    tempo_base.setWidth(82);
    tempo_base.setObjectName("align-center:direct-tempo-bpm");

    QQuickItem off_tile(&root);
    off_tile.setX(552);
    off_tile.setWidth(60);
    QQuickItem off_label(&off_tile);
    off_label.setWidth(60);
    off_label.setObjectName("align-x-parent");

    QQuickItem fog_tile(&root);
    fog_tile.setX(540);
    fog_tile.setWidth(348);
    QQuickItem fog_title(&fog_tile);
    fog_title.setX(112);
    fog_title.setWidth(124);
    fog_title.setObjectName("align-x-parent");
    QQuickItem left_icon(&fog_tile);
    left_icon.setX(74);
    left_icon.setWidth(28);
    left_icon.setObjectName("align-mirror:vertical-fog");
    QQuickItem right_icon(&fog_tile);
    right_icon.setX(246);
    right_icon.setWidth(28);
    right_icon.setObjectName("align-mirror:vertical-fog");
    if (desk_alignment_audit(&root) != 0)
        return 1;

    tempo_number.setX(559);
    if (desk_alignment_audit(&root) == 0)
        return 2;
    tempo_number.setX(552);
    off_label.setX(5);
    if (desk_alignment_audit(&root) == 0)
        return 3;
    off_label.setX(0);
    right_icon.setX(255);
    if (desk_alignment_audit(&root) == 0)
        return 4;
    return 0;
}
