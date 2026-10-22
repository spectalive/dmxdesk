#include "desk_view_audit.h"

#include <cstdio>

#include "desk_alignment_audit.h"
#include "desk_layout_audit.h"
#include "desk_text_audit.h"

int desk_view_audit(QQuickItem *root) {
    const int text = desk_text_audit(root);
    const int alignment = desk_alignment_audit(root);
    const int layout = desk_layout_audit(root);
    const int findings = text + alignment + layout;
    std::printf("@AGENT VERIFY %d\n", findings);
    std::fflush(stdout);
    return findings;
}
