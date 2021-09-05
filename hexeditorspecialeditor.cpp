#include "hexeditorspecialeditor.h"

HexEditorSpecialEditor::HexEditorSpecialEditor(QWidget *parent) : QTabWidget(parent)
{
    inspectorTab = new QWidget();
    analysisTab = new QWidget();

    addTab(analysisTab, "Analysis");
    addTab(inspectorTab, "Inspector");
}
