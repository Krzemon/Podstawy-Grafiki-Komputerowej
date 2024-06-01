#include <wx/wx.h>
#include <wx/colordlg.h>
#include <wx/filedlg.h> 

class StickFigurePanel : public wxPanel {
public:
    StickFigurePanel(wxWindow* parent)
        : wxPanel(parent), showImage(true), currentShape("Circle"), circleColor(*wxBLACK), otherElementsColor(*wxBLACK) {
        Bind(wxEVT_PAINT, &StickFigurePanel::OnPaint, this);
        Bind(wxEVT_SIZE, &StickFigurePanel::OnSizeChanged, this);
    }

    void SetStatusText(const wxString& text) {
        statusText = text;
        Refresh();
    }

    void ShowImage(bool show) {
        showImage = show;
        Refresh();
    }

    void SetShape(const wxString& shape) {
        currentShape = shape;
        Refresh();
    }

    void SetCircleColor(const wxColor& color) {
        circleColor = color;
        Refresh();
    }

    void SetOtherElementsColor(const wxColor& color) {
        otherElementsColor = color;
        Refresh();
    }

    void SaveImageToFile(const wxString& filename) {
        wxBitmap bitmap(GetSize());
        wxMemoryDC dc(bitmap);
        Render(dc);
        bitmap.SaveFile(filename, wxBITMAP_TYPE_PNG);
    }

private:
    wxString statusText;
    bool showImage;
    wxString currentShape;
    wxColor circleColor;
    wxColor otherElementsColor;

    void OnPaint(wxPaintEvent& event) {
        wxPaintDC dc(this);
        Render(dc);
    }

    void OnSizeChanged(wxSizeEvent& event) {
        Refresh();
        event.Skip();
    }

    void Render(wxDC& dc) {
        dc.Clear();

        wxSize panelSize = GetSize();
        int centerX = panelSize.GetWidth() / 2;
        int centerY = panelSize.GetHeight() / 2;

        dc.SetPen(wxPen(otherElementsColor, 2));
        dc.DrawLine(centerX, centerY - 50, centerX, centerY + 50);
        dc.DrawLine(centerX, centerY, centerX - 20, centerY + 30);
        dc.DrawLine(centerX, centerY, centerX + 20, centerY + 30);
        dc.DrawLine(centerX, centerY + 50, centerX - 20, centerY + 80);
        dc.DrawLine(centerX, centerY + 50, centerX + 20, centerY + 80);

        dc.SetBrush(wxBrush(*wxWHITE));
        dc.DrawCircle(centerX, centerY - 50, 25);

        wxCoord textWidth, textHeight;
        dc.GetTextExtent(statusText, &textWidth, &textHeight);
        dc.DrawText(statusText, centerX - textWidth / 2, centerY + 100);

        int shapeX, shapeY;

        if (currentShape == "Circle") {
            shapeX = centerX + 100;
            shapeY = centerY - 25;
            dc.SetBrush(wxBrush(circleColor));
            dc.DrawCircle(shapeX, shapeY, 50);
        }
        else if (currentShape == "Square") {
            shapeX = centerX + 100;
            shapeY = centerY - 25;
            dc.SetBrush(wxBrush(otherElementsColor));
            dc.DrawRectangle(shapeX, shapeY, 50, 50);
        }
        else if (currentShape == "Triangle") {
            shapeX = centerX + 100;
            shapeY = centerY - 25;
            wxPoint points[3];
            points[0] = wxPoint(centerX + 50, centerY - 50);
            points[1] = wxPoint(centerX + 150, centerY + 50);
            points[2] = wxPoint(centerX + 200, centerY - 50);
            dc.SetBrush(wxBrush(otherElementsColor));
            dc.DrawPolygon(3, points);
        }

        if (showImage) {
            wxImage image;
            //if (image.LoadFile(wxT("149.png"), wxBITMAP_TYPE_PNG)) {
              //  wxBitmap bitmap(image);
                //dc.DrawBitmap(bitmap, 100 - bitmap.GetWidth() / 2, 10, true);
            //}
        }
    }
};

class StickFigureFrame : public wxFrame {
public:
    StickFigureFrame()
        : wxFrame(NULL, wxID_ANY, "Stick Figure", wxDefaultPosition, wxSize(800, 600)) {
        panel = new StickFigurePanel(this);

        wxPanel* textPanel = new wxPanel(this);
        text_ctrl = new wxTextCtrl(textPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER | wxTE_MULTILINE);
        wxBoxSizer* textSizer = new wxBoxSizer(wxVERTICAL);
        textSizer->Add(text_ctrl, 1, wxEXPAND | wxALL, 5);
        textPanel->SetSizer(textSizer);

        wxBoxSizer* checkboxSizer = new wxBoxSizer(wxHORIZONTAL);
        checkbox = new wxCheckBox(textPanel, wxID_ANY, wxT("Poka  obrazek"), wxDefaultPosition, wxDefaultSize, 0);
        checkboxSizer->Add(checkbox, 0, wxALL, 5);
        textSizer->Add(checkboxSizer, 0, wxEXPAND | wxALL, 5);

        wxArrayString shapes;
        shapes.Add("Circle");
        shapes.Add("Square");
        shapes.Add("Triangle");

        shapeChoice = new wxChoice(textPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, shapes);
        textSizer->Add(shapeChoice, 0, wxEXPAND | wxALL, 5);

        colorButton = new wxButton(textPanel, wxID_ANY, "Zmie  kolor", wxDefaultPosition, wxDefaultSize);
        textSizer->Add(colorButton, 0, wxEXPAND | wxALL, 5);

        saveButton = new wxButton(textPanel, wxID_ANY, "Zapisz obrazek", wxDefaultPosition, wxDefaultSize);
        textSizer->Add(saveButton, 0, wxEXPAND | wxALL, 5);

        wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
        sizer->Add(textPanel, 0, wxEXPAND | wxALL, 5);
        sizer->Add(panel, 1, wxEXPAND | wxALL, 5);

        SetSizer(sizer);

        text_ctrl->Bind(wxEVT_TEXT, &StickFigureFrame::OnTextEntered, this);
        checkbox->Bind(wxEVT_CHECKBOX, &StickFigureFrame::OnCheckboxClicked, this);
        shapeChoice->Bind(wxEVT_CHOICE, &StickFigureFrame::OnShapeSelected, this);
        colorButton->Bind(wxEVT_BUTTON, &StickFigureFrame::OnColorButtonClicked, this);
        saveButton->Bind(wxEVT_BUTTON, &StickFigureFrame::OnSaveButtonClicked, this);

        Bind(wxEVT_SIZE, &StickFigureFrame::OnSizeChanged, this);
    }

private:
    StickFigurePanel* panel;
    wxTextCtrl* text_ctrl;
    wxCheckBox* checkbox;
    wxChoice* shapeChoice;
    wxButton* colorButton;
    wxButton* saveButton;

    void OnTextEntered(wxCommandEvent& event) {
        wxString text = text_ctrl->GetValue();
        panel->SetStatusText(text);
    }

    void OnCheckboxClicked(wxCommandEvent& event) {
        bool isChecked = checkbox->GetValue();
        panel->ShowImage(isChecked);
    }

    void OnShapeSelected(wxCommandEvent& event) {
        wxString shape = shapeChoice->GetStringSelection();
        panel->SetShape(shape);
    }

    void OnColorButtonClicked(wxCommandEvent& event) {
        wxColourDialog dialog(this);
        if (dialog.ShowModal() == wxID_OK) {
            wxColourData data = dialog.GetColourData();
            wxColour color = data.GetColour();
            if (shapeChoice->GetStringSelection() == "Circle") {
                panel->SetCircleColor(color);
            }
            else {
                panel->SetOtherElementsColor(color);
            }
        }
    }

    void OnSaveButtonClicked(wxCommandEvent& event) {
        wxFileDialog saveFileDialog(this, _("Zapisz obrazek do pliku"), "", "", "PNG files (*.png)|*.png", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

        if (saveFileDialog.ShowModal() == wxID_CANCEL)
            return;

        wxString filename = saveFileDialog.GetPath();
        panel->SaveImageToFile(filename);
    }

    void OnSizeChanged(wxSizeEvent& event) {
        panel->Refresh();
        event.Skip();
    }

    void OnClose(wxCloseEvent& event) {
        Destroy();
    }

    wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(StickFigureFrame, wxFrame)
EVT_CLOSE(StickFigureFrame::OnClose)
wxEND_EVENT_TABLE()

class MyApp : public wxApp {
public:
    virtual bool OnInit() {

        wxImage::AddHandler(new wxPNGHandler);

        StickFigureFrame* frame = new StickFigureFrame();
        frame->Show();
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
