#ifndef __GUIMyFrame1__
#define __GUIMyFrame1__

#include "GUI.h"

#include <wx/filedlg.h>
#include <wx/dcmemory.h>
#include <wx/dcclient.h>
#include <wx/dcbuffer.h>
#include <wx/colourdata.h>
#include <wx/colordlg.h>
#include <memory>

class ConfigClass;

/** Implementing MyFrame1 */
class GUIMyFrame1 : public MyFrame1
{
	protected:
		// Handlers for MyFrame1 events.
		void MainFormClose( wxCloseEvent& event );
		void WxPanel_Repaint( wxUpdateUIEvent& event );
		void WxEdit_x0_Update( wxCommandEvent& event );
		void WxEdit_y0_Update( wxCommandEvent& event );
		void WxEdit_x1_Update( wxCommandEvent& event );
		void WxEdit_y1_Update( wxCommandEvent& event );
		void WxScrollBar_alpha_Update( wxScrollEvent& event );
		void WxRB_Middle_Click( wxCommandEvent& event );
		void WxRB_Center_Click( wxCommandEvent& event );
		void WxEdit_dX_Update( wxCommandEvent& event );
		void WxEdit_dY_Update( wxCommandEvent& event );
		void WxEdit_x_start_Update( wxCommandEvent& event );
		void WxEdit_x_stop_Update( wxCommandEvent& event );
		void WxChoice_Selected( wxCommandEvent& event );
		void m_button1_click( wxCommandEvent& event );
		void m_button2_click( wxCommandEvent& event );
		void m_button3_click( wxCommandEvent& event );
	public:
		/** Constructor */
		GUIMyFrame1( wxWindow* parent );
	//// end generated class members
  void Repaint();
  void UpdateControls();
  ~GUIMyFrame1();
  std::shared_ptr<ConfigClass> cfg;
};

#endif // __GUIMyFrame1__
