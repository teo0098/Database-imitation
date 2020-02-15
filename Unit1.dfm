object DB: TDB
  Left = 0
  Top = 0
  BiDiMode = bdLeftToRight
  Caption = 'DB'
  ClientHeight = 806
  ClientWidth = 1478
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  ParentBiDiMode = False
  Position = poDesigned
  WindowState = wsMaximized
  PixelsPerInch = 96
  TextHeight = 13
  object Panel10: TPanel
    Left = 1063
    Top = 22
    Width = 400
    Height = 755
    BevelKind = bkFlat
    BevelOuter = bvNone
    BiDiMode = bdLeftToRight
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentBiDiMode = False
    ParentBackground = False
    ParentFont = False
    TabOrder = 0
    object GroupBox1: TGroupBox
      Left = 6
      Top = 6
      Width = 387
      Height = 435
      BiDiMode = bdLeftToRight
      Caption = 'Add to database'
      Color = clAppWorkSpace
      ParentBackground = False
      ParentBiDiMode = False
      ParentColor = False
      TabOrder = 0
      object Title: TEdit
        Left = 16
        Top = 72
        Width = 153
        Height = 21
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        Text = 'Title'
      end
      object ReleaseYear: TEdit
        Left = 16
        Top = 112
        Width = 153
        Height = 21
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        MaxLength = 4
        ParentFont = False
        TabOrder = 1
        Text = 'ReleaseYear'
      end
      object Amount: TEdit
        Left = 16
        Top = 152
        Width = 153
        Height = 21
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        MaxLength = 9
        ParentFont = False
        TabOrder = 2
        Text = 'Amount'
      end
      object Rewarded: TCheckBox
        Left = 216
        Top = 34
        Width = 121
        Height = 17
        Caption = 'Rewarded'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 3
      end
      object Price: TEdit
        Left = 216
        Top = 72
        Width = 153
        Height = 21
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        MaxLength = 9
        ParentFont = False
        TabOrder = 4
        Text = 'Price'
      end
      object Rating: TEdit
        Left = 216
        Top = 112
        Width = 153
        Height = 21
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        MaxLength = 9
        ParentFont = False
        TabOrder = 5
        Text = 'Rating'
      end
      object Description: TEdit
        Left = 216
        Top = 152
        Width = 153
        Height = 21
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 6
        Text = 'Description'
      end
      object Add: TButton
        Left = 16
        Top = 179
        Width = 153
        Height = 34
        Cursor = crHandPoint
        Caption = 'Add'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 7
        OnClick = AddClick
      end
      object Search: TButton
        Left = 16
        Top = 353
        Width = 153
        Height = 33
        Cursor = crHandPoint
        Caption = 'Linear search'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 8
        OnClick = SearchClick
      end
      object Searchings: TComboBox
        Left = 216
        Top = 305
        Width = 153
        Height = 21
        Style = csDropDownList
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ItemIndex = 0
        ParentFont = False
        TabOrder = 9
        Text = 'Performer'
        Items.Strings = (
          'Performer'
          'Title'
          'Year'
          'Amount'
          'Reward'
          'Price'
          'Rating'
          'Description')
      end
      object Back: TButton
        Left = 16
        Top = 305
        Width = 153
        Height = 34
        Cursor = crHandPoint
        Caption = 'Bring back data'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 10
        OnClick = BackClick
      end
      object BSearch: TButton
        Left = 216
        Top = 353
        Width = 153
        Height = 34
        Cursor = crHandPoint
        Caption = 'Binary search'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 11
        OnClick = BSearchClick
      end
      object MostR: TButton
        Left = 16
        Top = 241
        Width = 153
        Height = 34
        Cursor = crHandPoint
        Caption = 'Most repeated value'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 12
        OnClick = MostRClick
      end
      object Inverted: TButton
        Left = 16
        Top = 393
        Width = 153
        Height = 34
        Cursor = crHandPoint
        Caption = 'Inverted list search'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 13
        OnClick = InvertedClick
      end
      object ChainList: TButton
        Left = 216
        Top = 393
        Width = 153
        Height = 34
        Cursor = crHandPoint
        Caption = 'Chain list search'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 14
        OnClick = ChainListClick
      end
    end
    object GroupBox2: TGroupBox
      Left = 8
      Top = 447
      Width = 385
      Height = 122
      Caption = 'Delete'
      Color = clAppWorkSpace
      ParentBackground = False
      ParentColor = False
      TabOrder = 1
      object IDD: TEdit
        Left = 16
        Top = 32
        Width = 153
        Height = 21
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        MaxLength = 5
        ParentFont = False
        TabOrder = 0
        Text = 'Id'
      end
      object Delete: TButton
        Left = 216
        Top = 24
        Width = 153
        Height = 34
        Cursor = crHandPoint
        Caption = 'Delete'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 1
        OnClick = DeleteClick
      end
      object DeleteAll: TButton
        Left = 16
        Top = 72
        Width = 153
        Height = 34
        Cursor = crHandPoint
        Caption = 'Delete all'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 2
        OnClick = DeleteAllClick
      end
      object DeletePer: TButton
        Left = 216
        Top = 72
        Width = 153
        Height = 34
        Cursor = crHandPoint
        Caption = 'Delete per given id'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 3
        OnClick = DeletePerClick
      end
    end
    object GroupBox3: TGroupBox
      Left = 8
      Top = 575
      Width = 385
      Height = 98
      Caption = 'Export/Import'
      Color = clAppWorkSpace
      ParentBackground = False
      ParentColor = False
      TabOrder = 2
      object Export: TButton
        Left = 16
        Top = 32
        Width = 153
        Height = 34
        Cursor = crHandPoint
        Caption = 'Export to file'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
        OnClick = ExportClick
      end
      object Import: TButton
        Left = 216
        Top = 32
        Width = 153
        Height = 34
        Cursor = crHandPoint
        Caption = 'Import from file'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 1
        OnClick = ImportClick
      end
      object Error: TEdit
        Left = 16
        Top = 72
        Width = 353
        Height = 21
        Enabled = False
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clRed
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ReadOnly = True
        TabOrder = 2
        Visible = False
      end
    end
    object GroupBox4: TGroupBox
      Left = 8
      Top = 679
      Width = 385
      Height = 66
      Caption = 'Draw'
      Color = clAppWorkSpace
      ParentBackground = False
      ParentColor = False
      TabOrder = 3
      object Drawing: TEdit
        Left = 16
        Top = 32
        Width = 153
        Height = 21
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        MaxLength = 6
        ParentFont = False
        TabOrder = 0
        Text = 'Amount'
      end
      object Draw: TButton
        Left = 216
        Top = 21
        Width = 153
        Height = 34
        Cursor = crHandPoint
        Caption = 'Do some drawing'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 1
        OnClick = DrawClick
      end
    end
    object Performer: TComboBox
      Left = 24
      Top = 40
      Width = 153
      Height = 21
      Style = csDropDownList
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ItemIndex = 0
      ParentFont = False
      TabOrder = 4
      Text = 'John Simmons'
      Items.Strings = (
        'John Simmons'
        'Ronald Ponder'
        'Glenn McIntyre'
        'Brandon Almond'
        'Bernardo Rivers'
        'Michele Green'
        'Edward Greer'
        'Nicole Coomer'
        'Chris Mendenhall'
        'Paul Jasinski'
        'Jack Campbell'
        'David Brown'
        'Timothy Duncan'
        'Michelle Mann'
        'Cruz Davis'
        'James Kenyon'
        'Maura Book'
        'Paul Judd'
        'Gladys Hatley'
        'Joyce Harrison'
        'Robert Brown'
        'Nicole Hawkins'
        'Olga Lester'
        'Gena Shepherd'
        'Scott Meyer'
        'Jennifer Bridges'
        'Carlos Roberts'
        'Nena Underwood'
        'Maryann Ray'
        'Daniel Boyd')
    end
  end
  object Table: TStringGrid
    Left = 20
    Top = 22
    Width = 1037
    Height = 755
    Color = clBlack
    ColCount = 9
    FixedColor = clBlack
    RowCount = 1
    FixedRows = 0
    GradientEndColor = clBlack
    GradientStartColor = clBlack
    TabOrder = 1
    RowHeights = (
      31)
  end
end
