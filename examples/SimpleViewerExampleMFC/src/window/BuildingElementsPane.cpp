// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

#include "stdafx.h"
#include "MainFrame.h"
#include "BuildingElementsPane.h"
#include "Resource.h"
#include "BuildingUtils.h"
#include "IfcQueryDoc.h"
#include "IfcQueryMFC.h"

class CClassViewMenuButton : public CMFCToolBarMenuButton
{
	friend class BuildingElementsPane;

	DECLARE_SERIAL(CClassViewMenuButton)

public:
	CClassViewMenuButton(HMENU hMenu = nullptr) : CMFCToolBarMenuButton((UINT)-1, hMenu, -1)
	{
	}

	virtual void OnDraw(CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, BOOL bHorz = TRUE,
		BOOL bCustomizeMode = FALSE, BOOL bHighlight = FALSE, BOOL bDrawBorder = TRUE, BOOL bGrayDisabledButtons = TRUE)
	{
		pImages = CMFCToolBar::GetImages();

		CAfxDrawState ds;
		pImages->PrepareDrawImage(ds);

		CMFCToolBarMenuButton::OnDraw(pDC, rect, pImages, bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);

		pImages->EndDrawImage(ds);
	}
};

IMPLEMENT_SERIAL(CClassViewMenuButton, CMFCToolBarMenuButton, 1)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BuildingElementsPane::BuildingElementsPane()
{
	m_nCurrSort = ID_SORTING_GROUPBYTYPE;
}

BuildingElementsPane::~BuildingElementsPane()
{
}

BEGIN_MESSAGE_MAP(BuildingElementsPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_CLASS_ADD_MEMBER_FUNCTION, OnClassAddMemberFunction)
	ON_COMMAND(ID_CLASS_ADD_MEMBER_VARIABLE, OnClassAddMemberVariable)
	ON_COMMAND(ID_CLASS_DEFINITION, OnClassDefinition)
	ON_COMMAND(ID_CLASS_PROPERTIES, OnClassProperties)
	ON_COMMAND(ID_NEW_FOLDER, OnNewFolder)
	//ON_NOTIFY( TVN_SELCHANGED, IDC_MATERIAL_PROP_TREE, OnSelChangedTreeCtrl )
	//ON_NOTIFY( TVN_SELCHANGED, OnSelChangedTreeCtrl )
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_COMMAND_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnSort)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnUpdateSort)
END_MESSAGE_MAP()

//TVN_SELCHANGED


/////////////////////////////////////////////////////////////////////////////
// CClassView message handlers

int BuildingElementsPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create views:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndBuildingElmentsTree.Create(dwViewStyle, rectDummy, this, 2))
	{
		TRACE0("Failed to create building elements view\n");
		return -1;      // fail to create
	}

	// Load images:
	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_SORT);
	m_wndToolBar.LoadToolBar(IDR_SORT, 0, 0, TRUE /* Is locked */);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	CMenu menuSort;
	menuSort.LoadMenu(IDR_POPUP_SORT);

	m_wndToolBar.ReplaceButton(ID_SORT_MENU, CClassViewMenuButton(menuSort.GetSubMenu(0)->GetSafeHmenu()));

	CClassViewMenuButton* pButton =  DYNAMIC_DOWNCAST(CClassViewMenuButton, m_wndToolBar.GetButton(0));

	if (pButton != nullptr)
	{
		pButton->m_bText = FALSE;
		pButton->m_bImage = TRUE;
		pButton->SetImage(GetCmdMgr()->GetCmdImage(m_nCurrSort));
		pButton->SetMessageWnd(this);
	}

	// Fill in some static tree view data (dummy code, nothing magic here)
	updateBuildingElementsTreeView();


	return 0;
}

void BuildingElementsPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void BuildingElementsPane::setBuildingModel(shared_ptr<BuildingModel> ifc_model)
{
	m_ifc_model = ifc_model;
}

void BuildingElementsPane::updateBuildingElementsTreeView()
{
	clearBuildingElementsTreeView();
	if( !m_ifc_model )
	{
		updateBuildingElementsTreeViewDummy();
		return;
	}

	shared_ptr<IfcProject> ifc_project = m_ifc_model->getIfcProject();
	if( !ifc_project )
	{
		updateBuildingElementsTreeViewDummy();
		return;
	}

	std::wstringstream strs;
	strs << "#" << ifc_project->m_entity_id << "=" << ifc_project->className();
	HTREEITEM project_item = m_wndBuildingElmentsTree.InsertItem( strs.str().c_str(), 0, 0 );
	m_wndBuildingElmentsTree.SetItemState( project_item, TVIS_BOLD, TVIS_BOLD );
	m_map_id_item[ifc_project->m_entity_id] = project_item;

	std::vector<shared_ptr<IfcObjectDefinition> > vec_sites;
	BuildingUtils::getRelatedElements( ifc_project, vec_sites );

	for( auto site : vec_sites )
	{
		if( !site )
		{
			continue;
		}

		std::wstringstream strs;
		strs << "#" << site->m_entity_id << "=" << site->className();
		HTREEITEM site_item = m_wndBuildingElmentsTree.InsertItem( strs.str().c_str(), 1, 1, project_item );
		m_wndBuildingElmentsTree.Expand( project_item, TVE_EXPAND );
		m_map_id_item[site->m_entity_id] = site_item;

		std::vector<shared_ptr<IfcObjectDefinition> > vec_buildings;
		BuildingUtils::getRelatedElements( site, vec_buildings );

		for( auto building : vec_buildings )
		{
			if( !building )
			{
				continue;
			}
			std::wstringstream strs;
			strs << "#" << building->m_entity_id << "=" << building->className();
			HTREEITEM building_item = m_wndBuildingElmentsTree.InsertItem( strs.str().c_str(), 1, 1, site_item );
			m_wndBuildingElmentsTree.Expand( site_item, TVE_EXPAND );
			m_map_id_item[building->m_entity_id] = building_item;

			std::vector<shared_ptr<IfcObjectDefinition> > vec_building_storeys;
			BuildingUtils::getRelatedElements( building, vec_building_storeys );

			for( auto building_storey : vec_building_storeys )
			{
				if( !building_storey )
				{
					continue;
				}

				std::wstringstream strs;
				strs << "#" << building_storey->m_entity_id << "=" << building_storey->className();
				HTREEITEM building_storey_item = m_wndBuildingElmentsTree.InsertItem( strs.str().c_str(), 1, 1, building_item );
				m_wndBuildingElmentsTree.Expand( building_item, TVE_EXPAND );
				m_map_id_item[building_storey->m_entity_id] = building_storey_item;

				std::vector<shared_ptr<IfcObjectDefinition> > vec_products;
				BuildingUtils::getRelatedElements( building_storey, vec_products );

				for( auto product : vec_products )
				{
					if( !product )
					{
						continue;
					}

					std::wstringstream strs;
					strs << "#" << product->m_entity_id << "=" << product->className();
					HTREEITEM building_part_item = m_wndBuildingElmentsTree.InsertItem( strs.str().c_str(), 1, 1, building_storey_item );
					m_map_id_item[product->m_entity_id] = building_part_item;

					std::vector<shared_ptr<IfcObjectDefinition> > vec_products2;
					BuildingUtils::getRelatedElements( product, vec_products2 );

					for( auto product2 : vec_products2 )
					{
						if( !product2 )
						{
							continue;
						}

						std::wstringstream strs;
						strs << "#" << product2->m_entity_id << "=" << product2->className();
						HTREEITEM building_part_item2 = m_wndBuildingElmentsTree.InsertItem( strs.str().c_str(), 1, 1, building_part_item );
						m_map_id_item[product2->m_entity_id] = building_part_item2;

						std::vector<shared_ptr<IfcObjectDefinition> > vec_products3;
						BuildingUtils::getRelatedElements( product2, vec_products3 );

						for( auto product3 : vec_products3 )
						{
							if( !product3 )
							{
								continue;
							}

							std::wstringstream strs;
							strs << "#" << product3->m_entity_id << "=" << product3->className();
							HTREEITEM building_part_item3 = m_wndBuildingElmentsTree.InsertItem( strs.str().c_str(), 1, 1, building_part_item2 );
							m_map_id_item[product2->m_entity_id] = building_part_item3;

						}
					}
					
				}

				m_wndBuildingElmentsTree.Expand( building_storey_item, TVE_EXPAND );
			}
		}
	}
}

void BuildingElementsPane::unselectAllEntities()
{
	HTREEITEM sel_item = m_wndBuildingElmentsTree.GetSelectedItem();
	if( sel_item )
	{
		m_wndBuildingElmentsTree.m_internal_call_to_select = true;
		m_wndBuildingElmentsTree.SetItemState( sel_item, 0, TVIS_SELECTED );
		m_wndBuildingElmentsTree.m_internal_call_to_select = false;
	}
}

void BuildingElementsPane::setEntitySelected( int entity_id )
{
	auto it_find = m_map_id_item.find( entity_id );
	if( it_find != m_map_id_item.end() )
	{
		m_wndBuildingElmentsTree.m_internal_call_to_select = true;
		HTREEITEM hbuilding_part = it_find->second;
		m_wndBuildingElmentsTree.Expand( hbuilding_part, TVE_EXPAND );
		m_wndBuildingElmentsTree.SelectItem( hbuilding_part );
		m_wndBuildingElmentsTree.SetItemState( hbuilding_part, TVIS_SELECTED|LVIS_FOCUSED, TVIS_SELECTED | LVIS_FOCUSED );
		m_wndBuildingElmentsTree.m_internal_call_to_select = false;
	}
	else
	{
		unselectAllEntities();
	}
}

void BuildingElementsPane::updateBuildingElementsTreeViewDummy()
{
	HTREEITEM hRoot = m_wndBuildingElmentsTree.InsertItem(_T("IfcProject"), 0, 0);
	m_wndBuildingElmentsTree.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
}

void BuildingElementsPane::clearBuildingElementsTreeView()
{
	m_wndBuildingElmentsTree.DeleteAllItems();
	m_map_id_item.clear();
}

void BuildingElementsPane::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndBuildingElmentsTree;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// Select clicked item:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != nullptr)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}

	pWndTree->SetFocus();
	CMenu menu;
	menu.LoadMenu(IDR_POPUP_SORT);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}
}

void BuildingElementsPane::AdjustLayout()
{
	if (GetSafeHwnd() == nullptr)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(nullptr, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndBuildingElmentsTree.SetWindowPos(nullptr, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

BOOL BuildingElementsPane::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}

void BuildingElementsPane::OnSort(UINT id)
{
	if (m_nCurrSort == id)
	{
		return;
	}

	m_nCurrSort = id;

	CClassViewMenuButton* pButton =  DYNAMIC_DOWNCAST(CClassViewMenuButton, m_wndToolBar.GetButton(0));

	if (pButton != nullptr)
	{
		pButton->SetImage(GetCmdMgr()->GetCmdImage(id));
		m_wndToolBar.Invalidate();
		m_wndToolBar.UpdateWindow();
	}
}

void BuildingElementsPane::OnUpdateSort(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(pCmdUI->m_nID == m_nCurrSort);
}

void BuildingElementsPane::OnClassAddMemberFunction()
{
	AfxMessageBox(_T("Add member function..."));
}

void BuildingElementsPane::OnClassAddMemberVariable()
{
	// TODO: Add your command handler code here
}

void BuildingElementsPane::OnClassDefinition()
{
	// TODO: Add your command handler code here
}

void BuildingElementsPane::OnClassProperties()
{
	// TODO: Add your command handler code here
}

void BuildingElementsPane::OnNewFolder()
{
	AfxMessageBox(_T("New Folder..."));
}

void BuildingElementsPane::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	m_wndBuildingElmentsTree.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void BuildingElementsPane::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndBuildingElmentsTree.SetFocus();
}

void BuildingElementsPane::OnChangeVisualStyle()
{
	m_cImageList.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_CLASS_VIEW_24 : IDB_CLASS_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("Can't load bitmap: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_cImageList.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_cImageList.Add(&bmp, RGB(255, 0, 0));

	m_wndBuildingElmentsTree.SetImageList(&m_cImageList, TVSIL_NORMAL);

	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_SORT_24 : IDR_SORT, 0, 0, TRUE /* Locked */);
}
