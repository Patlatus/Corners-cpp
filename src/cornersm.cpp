#include <string>
#include <iostream>
#include <time.h>

#define MAP_SIZE 8
int MV = MAP_SIZE - 1;
#define MAP_EMPTY 0
#define MAP_OPONENT 1
#define MAP_YOU 2
int map[MAP_SIZE][MAP_SIZE];
int cz, move;
bool YOU_First;
bool Improve = false;
struct TPoint {
  short int X, Y;
};
struct TStep {
  TPoint From, To;  
};
TPoint __fastcall Point( int a, int b )
{
  TPoint result;
  result.X = a;
  result.Y = b;
  return result;
}
TPoint Corners[12];
TPoint OCorners[12];
TPoint FreeSpaces[12], OFreeSpaces[12];
typedef TPoint TUVA[4];

TUVA CUVA;
void CUVAInit( )
{
  CUVA[0].X = 1;
  CUVA[0].Y = 0;
  CUVA[1].X = 0;
  CUVA[1].Y = 1;
  CUVA[2].X = -1;
  CUVA[2].Y = 0;
  CUVA[3].X = 0;
  CUVA[3].Y = -1;
}
bool __fastcall InBounds( int a, int b )
{ // чи задовольняє значення граничні умови
  bool result = ( a >= 0 ) && ( b >= 0 ) && ( a <= MV ) && ( b <= MV );
  return result;
}
bool __fastcall InBounds(TPoint p)
{ // чи задовольняє значення граничні умови
  return InBounds(p.X, p.Y);
}
bool __fastcall EmptyCell(TPoint p)
{ // чи порожня клітинка
  return (map[p.X][p.Y] == MAP_EMPTY);
}
TPoint __fastcall AStep(int X, int Y, int k)
{
  TPoint Result;
  Result.X = X + CUVA[k].X;
  Result.Y = Y + CUVA[k].Y;
  return Result;
}
bool __fastcall isSingleStepPosible(int X, int Y, int k)
{ 
  TPoint a = AStep(X, Y, k);
//  int c = X + CUVA[k].X;
// int d = Y + CUVA[k].Y;
//  bool Result = ((InBounds(c, d)) && ((map[c][d] == MAP_EMPTY)));
  bool Result = (InBounds(a) && EmptyCell(a));
  return Result;
};
TPoint __fastcall AJump(int X, int Y, int k)
{
  TPoint Result;
  Result.X = X + 2 * CUVA[k].X;
  Result.Y = Y + 2 * CUVA[k].Y;
  return Result;
}
bool __fastcall isSingleJumpPosible(int X, int Y, int k)
{ 
  TPoint a = AStep(X, Y, k);
  TPoint b = AJump(X, Y, k);
//  int a = X + 2 * CUVA[k].X;
//  int b = Y + 2 * CUVA[k].Y;
//  int c = X + CUVA[k].X;
//  int d = Y + CUVA[k].Y;
//  bool Result = ((InBounds(a, b)) && (map[a][b] == MAP_EMPTY) && (!(map[c][d] == MAP_EMPTY)));
  bool Result = ((InBounds(b)) && (EmptyCell(b)) && (!(EmptyCell(a))));
  return Result;
};

bool __fastcall isStepPosible()
{
  for (int i=0; i<12; i++)
	  // цикл по фішках
  {
  for (int j=0; j<4; j++)
	  // цикл по напрямках
  {
	  if ((isSingleStepPosible(Corners[i].X, Corners[i].Y, j))||(isSingleJumpPosible(Corners[i].X, Corners[i].Y, j)))
		  return true;
  }
  }
  return false;
};
int __fastcall QuasiManhattanDistance(TPoint a, TPoint b)
{
	int Result = (b.Y - a.Y) + b.X - a.X;
  if (!(cz == MAP_YOU))
	{
		Result =  - Result;
		//std::cout << "a: "  << a.X  << a.Y << " b: "  << b.X  << b.Y << " QMD: " << Result << std::endl;  
	};
	return Result;
}
void __fastcall Init()
{
	CUVAInit( );
	// ініціалізація напрямків
	int k=0;
	for (int i=0; i<=2; i++)
	{
	for (int j=0; j<=3; j++)
	{
        //if (YOU_First)
		{
  		map[i][j] = MAP_YOU;
		Corners[k] = Point(i, j);
      OFreeSpaces[k] = Point( i, j );
		k++;
		}
		/*else
		{
  		map[i][j] = MAP_OPONENT;
		OCorners[k] = Point(i,j);
		k++;
		}*/
	}
	}
	for (int i=3; i<MAP_SIZE; i++)
	{
	for (int j=0; j<=3; j++)
	{
		map[i][j] = MAP_EMPTY;
	}
	}
	for (int i=0; i<=4; i++)
	{
	for (int j=4; j<MAP_SIZE; j++)
	{
		map[i][j] = MAP_EMPTY;
	}
	}
	k=0;
	for (int i=5; i<MAP_SIZE; i++)
	{
	for (int j=4; j<MAP_SIZE; j++)
	{
        //if (YOU_First)
		{
		map[i][j] = MAP_OPONENT;
		OCorners[k] = Point(i, j);
      FreeSpaces[k] = Point( i, j );
		k++;
		}
		/*else
		{
  		map[i][j] = MAP_YOU;
		Corners[k] = Point(i,j);
		k++;
		}*/
	}
	}
}
void doMove(int fromX, int fromY,int toX,int toY)
{
	
 /*
  if (!(map[fromX][fromY] == cz))
  {
	  //throw "!(map[fromX][fromY] == cz)";
	  std::cout << "!(map[fromX][fromY] == cz)" << std::endl;  
	  std::cout << "Error: " << fromX << " " << fromY << " " << toX << " " << toY << std::endl;  
	  std::cout << "cz: " << cz << " map[fromX][fromY]: " << map[fromX][fromY] << std::endl;  

  }
  if (!(map[toX][toY] == MAP_EMPTY))
  {
	  
	  std::cout << "!(map[toX][toY] == MAP_EMPTY)" << std::endl;  
	  std::cout << "Error: " << fromX << " " << fromY << " " << toX << " " << toY << std::endl;  
	  std::cout << "cz: " << cz << " map[fromX][fromY]: " << map[fromX][fromY] << std::endl;  

  }
*/
  map[toX][toY] = map[fromX][fromY];//cz;
  map[fromX][fromY] = MAP_EMPTY;

  if (cz == MAP_YOU)
  {
	  for (int k=0; k<12; k++)
	  {
		  if ((Corners[k].X == fromX)&&(Corners[k].Y == fromY))
		  {
			  Corners[k].X = toX; 
			  Corners[k].Y = toY; 
			  //break;
			  return;
		  }

	  }
	  //std::cout << "!error" << std::endl;  
	  
  }
  else
 { 
	  for (int k=0; k<12; k++)
	  {
		  if ((OCorners[k].X == fromX)&&(OCorners[k].Y == fromY))
		  {
			  OCorners[k].X = toX; 
			  OCorners[k].Y = toY; 
			  //break;
			  return;
		  }

	  }
	  //std::cout << "!error" << std::endl;  
	  
  }

};

bool __fastcall PointsEqual( TPoint a, TPoint b )
{
	return (a.X == b.X) && (a.Y == b.Y);
}

/*
struct TArr{
  short int Len;
  char Arr[100];
};
struct TGraphAL{
  short int Len;
  TArr Arr[100];
};
  enum TGA_Color {GA_clWhite,
                  GA_clGray,
                  GA_clBlack };
int G_CurrentIndex;
TArr Color;
TArr ASortedIndexMap;
void __fastcall LDFS_Visit(TGraphAL g, int u)
{
  int i = 0;
  int v;
  Color.Arr[u] = GA_clGray;
//  if ( (g[u].Len == 0 ) && ( Finish != u ) ) 
//    FConnected = false;
  for ( i = 0; i <= g.Arr[u].Len - 1; i++)
  {
	  v = g.Arr[u].Arr[i];
    switch ( Color.Arr[v] )
    {
      case GA_clWhite:
        LDFS_Visit(g, v);
      break;
      case GA_clGray:
        //throw "Topology_Sort() -> LLDFS_Visit(): Cycle FOUND!\x0d\x0a Graph is not directly acyclic!";
  	    //std::cout << "Topology_Sort() -> LLDFS_Visit(): Cycle FOUND!\x0d\x0a Graph is not directly acyclic!" << std::endl;  


      break;
    }
  }
  Color.Arr[u] = GA_clBlack;
  ASortedIndexMap.Arr[G_CurrentIndex] = u;
  G_CurrentIndex--;
}


void __fastcall  Topology_Sort(TGraphAL g)
{


  G_CurrentIndex = 0;
  Color.Len = g.Len;
  ASortedIndexMap.Len = g.Len;
  int i = 0;
  for ( i = 0; i <= g.Len - 1; i++)
    Color.Arr[i] = GA_clWhite;
  G_CurrentIndex = g.Len - 1;
//  FConnected = true;
  LDFS_Visit(g, 0 ); //FStart
  for ( i = 0; i <= g.Len - 1; i++)
    if ( Color.Arr[i] == GA_clWhite ) 
    {
      //FConnected = false;
	  LDFS_Visit(g, i);
    }
  Color.Len = 0;
}*/


TStep __fastcall Step(TPoint a, TPoint b)

{
  TStep result;
  result.From = a;
  result.To = b;
  return result;
}


TPoint __fastcall MiddlePoint( TPoint c, TPoint p )
{
  TPoint result;
  result.X = ( c.X + p.X ) / 2;
  result.Y = ( c.Y + p.Y ) / 2;
  return result;
}


int __fastcall ManhattanDistance(TPoint a, TPoint b)
{
  return abs(b.Y - a.Y) + abs(b.X - a.X);
}

bool __fastcall IsJumpPosible( TPoint c, TPoint p )
{
  bool result = false;
  result = ( ManhattanDistance( c, p ) == 2 ) && ( ( c.X == p.X ) || ( c.Y == p.Y ) ) && ( ! ( EmptyCell( MiddlePoint( c, p ) ) ) );
  return result;
}


bool __fastcall OwnsLongCorners( TPoint p, TPoint ACorners[12])
{
  bool result = false;
  unsigned char i = '\0'; // чи належить точка гравцю
  for ( i = 0; i <= 11; i++)
    //if (Corners[i].X = p.X) and (Corners[i].Y = p.Y) then
    if ( PointsEqual( ACorners[i], p ) ) 
    {
      result = true;
      return result;
    }
  result = false;
  return result;
}

/*bool __fastcall OwnsLong( TPoint p )
{
  bool result = false; // чи належить точка гравцю
  if ( cz == MAP_YOU ) 
    result = OwnsLongCorners( p, Corners, MAXIDX( Corners ) );
  else
    result = OwnsLongCorners( p, OCorners, MAXIDX( OCorners ) );
  return result;
}*/


bool __fastcall Owns( TPoint p )
{
  bool result = false; // чи належить точка гравцю
//  Result := YOU_First and (map[p.X][p.Y] = MAP_YOU) or not YOU_First and (map[p.X][p.Y] = MAP_OPONENT);
//  Result := (cz = MAP_YOU) and (map[p.X][p.Y] = MAP_YOU) or not (cz = MAP_YOU) and (map[p.X][p.Y] = MAP_OPONENT);
  result = ( cz == map[p.X][p.Y] );
  return result;
}


bool __fastcall IsSingleJumpExpected( int X, int Y, int k )
// REQ_0000: Preparation for long step: end of step

{
  bool result = false;
  TPoint a, b;
  bool OwnsB = false;
  a = AStep( X, Y, k );
  b = AJump( X, Y, k );
  OwnsB = Owns(b);//OwnsLong( b );
  result = ( ( InBounds( b ) ) && ( EmptyCell( b ) || OwnsB ) && ( ! ( EmptyCell( a ) ) ) );
  return result;
}
/* 
double __fastcall Duration( TArrayOfInteger& ASortedIndexMap, TArrayOfInteger& ACriticalPathArray, TArrayOfDouble& E, TArrayOfDouble& L, double& AStartPoint, double& AFinishPoint )
{
  double result = 0.0;
  const double PosInfty = 1e100;
  const double NegInfty = - PosInfty;
  int i = 0, j = 0, u = 0, v = 0;
  TWF_WorkflowItem LItem;
  Topology_Sort( ASortedIndexMap );
  SetLength( E, WorkflowItemList.Count );
  SetLength( ACriticalPathArray, WorkflowItemList.Count );
  for ( i = 0; i <= WorkflowItemList.Count - 1; i++)
  {
    E[i] = NegInfty;
    ACriticalPathArray[i] = - 1;
  }
  E[Start.Index] = 0;
  for ( i = 0; i <= WorkflowItemList.Count - 2; i++)
  {
    u = ASortedIndexMap[i];
    if ( E[u] == NegInfty ) 
      Continue;
    LItem = WorkflowItemList[u];
    for ( j = 0; j <= LItem.NextItemsList.Count - 1; j++)
    {
      v = LItem.NextItemsList[j].Index;
      if ( MTH_CompareValue( E[u] + ItemDuration( u ), E[v] ) > 0 ) 
      {
        E[v] = E[u] + ItemDuration( u );
        ACriticalPathArray[v] = u;
      }
    }
  }
  result = E[Finish.Index];
  SetLength( L, WorkflowItemList.Count );
  for ( i = 0; i <= WorkflowItemList.Count - 1; i++)
    L[i] = PosInfty;
  L[Finish.Index] = result;
  for ( i = WorkflowItemList.Count - 2; i >= 0; i--)
  {
    u = ASortedIndexMap[i];
    if ( E[u] == NegInfty ) 
      Continue;
    LItem = WorkflowItemList[u];
    for ( j = 0; j <= LItem.NextItemsList.Count - 1; j++)
    {
      v = LItem.NextItemsList[j].Index;
      if ( MTH_CompareValue( L[v] - ItemDuration( v ), L[u] ) < 0 ) 
        L[u] = L[v] - ItemDuration( v );
    }
  }
  AStartPoint = 0;
  AFinishPoint = result;
  switch ( ZeroOperationIndex )
  {
    case WF_OPERATION_ID_START:
      return result;
    break;
    case WF_OPERATION_ID_FINISH:
      DoZeroOffset( E, L, AStartPoint, AFinishPoint, FFinish.Index );
    break;
  default:
    if ( ( ZeroOperationIndex < 0 ) || ( ZeroOperationIndex >= WorkflowItemList.Count ) ) 
      throw Exception.Create( "" );
    else
      DoZeroOffset( E, L, AStartPoint, AFinishPoint, ZeroOperationIndex );
  }
  return result;
}
*/
bool __fastcall Compare(TPoint a, int p, TPoint b, int q)
{
  if (p != q)
  {
    return p > q;
  }
  if (a.X + a.Y != b.X + b.Y)
  {
    return a.X + a.Y > b.X + b.Y;
  }
  return a.X > b.X;
}

bool IsBetter(TPoint a, int p, TPoint b, int q)
{
	//bool Result = (Compare(a, p, b, q) == YOU_First);
	//return Result;

  if (p != q)
  {
    return p > q;
  }
  if (a.X + a.Y != b.X + b.Y)
  {
   return (a.X + a.Y > b.X + b.Y) == ( cz == MAP_YOU );
  }
  if (a.X != b.X)
  {
    return (a.X > b.X) == ( cz == MAP_YOU );
  }
  return false;
}
bool PointIn(TPoint p, int ALen, TPoint Points[100])
{
for (int i=0; i<ALen; i++)
{
	if ((Points[i].X == p.X)&&(Points[i].Y == p.Y))
		return true;
}
return false;
}
TStep CurrentStep, AuxiStep;
typedef TPoint TPoints [100];
void RecursiveFindAllJumps(TPoint start, int *len, TPoint *Points)
//void __fastcall RecursiveFindAllJumps(TPoint Start, int& ALen, TPoint* Points)
{
  Points[*len] = start;
  (*len)++;
  int cx = start.X;
  int cy = start.Y;
  TPoint p;
  for (int j=0; j<4; j++)
     {  
	  if (isSingleJumpPosible(cx, cy, j))
	  {
		  p = AJump(cx, cy, j);
		  if (!(PointIn(p, *len, Points)))
			  RecursiveFindAllJumps(p, len, Points);
	  }

     }
}


void __fastcall RecursiveFindAllPossibleJumps( TPoint Start, int *len, TPoint *Points)
//void __fastcall RecursiveFindAllPossibleJumps( TPoint Start, int& ALen, TPoint* Points)
{
  int cx = 0, cy = 0, j = 0;
  TPoint p;
  Points[*len] = Start;
  (*len)++;
  cx = Start.X;
  cy = Start.Y;
  for ( j = 0; j <= 3; j++)
    if ( IsSingleJumpExpected( cx, cy, j ) ) 
    {
		p = AJump( cx, cy, j );
		if (!(PointIn(p, *len, Points)))
			RecursiveFindAllPossibleJumps(p, len, Points);
    }
}


int MaxPrice(TPoint c)
//int __fastcall MaxPrice( TPoint c )
// a -- індекс фішки

{
  int Result, PointsLen, cp;
  TPoint p;
  TPoint Points[100];
/*  char mapcopy[MAP_SIZE][MAP_SIZE];
	for (int i=0; i<MAP_SIZE; i++)
	{
	for (int j=0; j<MAP_SIZE; j++)
	{
		mapcopy[i][j] = map[i][j];
	}
	}
	mapcopy[c.X][c.Y] = MAP_EMPTY;
*/
	//c = Corners[a];
	CurrentStep.From.X = c.X;
	CurrentStep.From.Y = c.Y;
    Result = -15;

	PointsLen = 0;
	RecursiveFindAllJumps(c, &PointsLen, Points);
  //RecursiveFindAllJumps( c, PointsLen, Points );
    /* if (a == 2)
	{
		std::cout << "PointsLen " << PointsLen << std::endl; 
		for (int i=1; i<PointsLen; i++)
		{
		  p = Points[i];
		  std::cout << "p[" << i << "] " << p.X << p.Y << std::endl; 
		}
    }
	
	if (!(c.X == Corners[a].X)||!(c.Y == Corners[a].Y))
	{		
		//std::cout << "(!(c.X == Corners[a].X)||!(c.Y == Corners[a].Y)) " << c.X << " " << c.Y << " " << Corners[a].X << " " << Corners[a].Y << std::endl;  
}*/
	for (int i=1; i<PointsLen; i++)
	{
		  p = Points[i];
		  cp = QuasiManhattanDistance(c, p);
		  if (IsBetter(p, cp, CurrentStep.To, Result))//   (cp > Result)
		  {
			   Result = cp;
			   CurrentStep.To.X = p.X;
			   CurrentStep.To.Y = p.Y;
		  }
	}

	for (int j=0; j<4; j++)
    {  
	  if (isSingleStepPosible(c.X, c.Y, j))
	  {
		  p = AStep(c.X, c.Y, j);
		  cp = QuasiManhattanDistance(c, p);
		  if (IsBetter(p, cp, CurrentStep.To, Result))//if (cp > Result)
		  {
			   Result = cp;
			   CurrentStep.To.X = p.X;
			   CurrentStep.To.Y = p.Y;
		  }
	  }
    }
	return Result;
 

}

int __fastcall MaxExpectedPrice( TPoint c )
// REQ_0000: Preparation for long step: beginning of step

{
  int result = 0;
  int cp = 0, j = 0;
  TPoint p;
  unsigned char mc = '\0';
  AuxiStep.From.X = c.X;
  AuxiStep.From.Y = c.Y;
  result = - 15;
  mc = map[c.X][c.Y];
  map[c.X][c.Y] = 0;
  for ( j = 0; j <= 3; j++)
    if ( isSingleStepPosible( c.X, c.Y, j ) ) 
    {
      p = AStep( c.X, c.Y, j );
      cp = MaxPrice( p );
      if ( ( cp > 1 ) & ( IsBetter( p, cp, AuxiStep.To, result ) ) ) 
      {//if (cp > Result)
        result = cp;
        AuxiStep.To.X = p.X;
        AuxiStep.To.Y = p.Y;
      }
    }
  map[c.X][c.Y] = mc;
  return result;
}
bool Urgent(TPoint c)
{
//	TPoint c = Corners[i];
	/* bool Result;
	if (YOU_First)
	{
		Result = (c.X<=2)&&(c.Y<=3);
	}
	else
	{
		Result = (c.X>=5)&&(c.Y>=4);
	}
	return Result; */
  bool result = false;
  result = ! ( cz == MAP_YOU ) && ( c.X >= 5 ) && ( c.Y >= 4 ) || ( ( cz == MAP_YOU ) && ( c.X <= 2 ) && ( c.Y <= 3 ) );
  return result;
}
bool Ready(TPoint c)
{
//	TPoint c = Corners[i];
	/*bool Result;
	if (YOU_First)
	{
		Result = (c.X>=5)&&(c.Y>=4);
	}
	else
	{
		Result = (c.X<=2)&&(c.Y<=3);
	}
	return Result;*/
  bool result = false;
/*    if (YOU_First)
        Result := (c.X >= 5) and (c.Y >= 4);
    else
        Result := (c.X <= 2) and (c.Y <= 3);*/
//    Result := YOU_First and (c.X >= 5) and (c.Y >= 4) or not YOU_First and (c.X <= 2) and (c.Y <= 3);
  result = ( cz == MAP_YOU ) && ( c.X >= 5 ) && ( c.Y >= 4 ) || ( ! ( cz == MAP_YOU ) && ( c.X <= 2 ) && ( c.Y <= 3 ) );
  return result;
}
bool UrgentExists(TPoint ACorners[12])
{
	for (int i=0; i<12; i++)
	{
		if (Urgent(ACorners[i]))
		{
			return true;
		}
	}
	return false;
  
}
bool Win(TPoint ACorners[12])
//bool __fastcall Win( const TPoint* ACorners/*12*/, int ACorners_maxidx )
{
	for (int i=0; i<12; i++)
	{
		if (!Ready(ACorners[i]))
		{
			return false;
		}
	}
	return true;
  
}
bool EpicWin(int sign)
{
    bool Result;
    if (cz == MAP_YOU)
        Result = Win(Corners);
    else
    {
        YOU_First = !YOU_First;
        Result = Win(OCorners);
        YOU_First = !YOU_First;
    }
    return Result;
}
bool EpicWin()
{
    return (EpicWin(MAP_YOU) || EpicWin(MAP_OPONENT));
}
bool EpicFail()
{
    return (!EpicWin());
}

TStep doStep(TPoint ACorners[12])
{
  int mp = -15;
  int mpi;
  TStep BestStep;
  for (int i=0; i<12; i++)
  {
      if ((UrgentExists(ACorners)) && !Urgent(ACorners[i]))
	    continue;
	  //if (Ready(ACorners[i]))
	  //  continue;
	  mpi = MaxPrice(ACorners[i]);
	  //std::cout << "i: " << i << " mp: " << mp << " Corners[i]: " << Corners[i].X  << Corners[i].Y << " MaxPrice(i): " << mpi << " CurrentStep: " << CurrentStep.From.X << CurrentStep.From.Y << CurrentStep.To.X << CurrentStep.To.Y << std::endl;  

  if (IsBetter(CurrentStep.To, mpi, BestStep.To, mp))//if (mpi > mp)
  {
	  //std::cout << "Step: " << CurrentStep.From.X << CurrentStep.From.Y << CurrentStep.To.X << CurrentStep.To.Y << "[" << mpi << "] is better then step: " << BestStep.From.X << BestStep.From.Y << BestStep.To.X << BestStep.To.Y << "[" << mp << "]" << std::endl;  
	  mp = mpi;
      BestStep = CurrentStep;
  }
  }
  
  doMove(BestStep.From.X, BestStep.From.Y, BestStep.To.X, BestStep.To.Y); 
  return BestStep;
};



TStep __fastcall MakeStep( TPoint c, TPoint p, TPoint AExitPoints[12])
{
  TStep result;
  int d = 0;
  d = ManhattanDistance( c, p );
  if ( ( d == 1 ) || ( ( d == 2 ) & IsJumpPosible( c, p ) ) ) 
    result = Step( c, p );
  else
  {
    MaxPrice( c );
    result = CurrentStep;
  }
  return result;
}



TPoint __fastcall NearestExitPoint( TPoint c, TPoint AExitPoints[12], int ALen = 12 )
{
  TPoint result;
  int i = 0, j = 0, cp = 0, mincp = 0;
  j = 0;
  mincp = ManhattanDistance( c, AExitPoints[0] );
  for ( i = 1; i <= ALen - 1; i++)
  {
    cp = ManhattanDistance( c, AExitPoints[i] );
    if ( cp < mincp ) 
    {
      mincp = cp;
      j = i;
    }
  }
  result = AExitPoints[j];
  return result;
}


TPoint __fastcall NearestEmptyExitPoint( TPoint c, TPoint AExitPoints[12], int ALen = 12 )
{
  TPoint result;
  int i = 0, j = 0, cp = 0, mincp = 0;
  j = 0;
  mincp = 30;
  for ( i = 0; i <= ALen - 1; i++)
  {
    if ( ! EmptyCell( AExitPoints[i] ) ) 
      continue;
    cp = ManhattanDistance( c, AExitPoints[i] );
    if ( cp < mincp ) 
    {
      mincp = cp;
      j = i;
    }
  }
  result = AExitPoints[j];
  return result;
}


TPoint __fastcall NearestOwnsLongExitPoint( TPoint c, TPoint AExitPoints[12], int ALen = 12 )
{
  TPoint result;
  int i = 0, j = 0, cp = 0, mincp = 0;
  j = 0;
  mincp = 30;
  for ( i = 0; i <= ALen - 1; i++)
  {
    if (!Owns(AExitPoints[i]))// if ( ! OwnsLong( AExitPoints[i] ) ) 
      continue;
    cp = ManhattanDistance( c, AExitPoints[i] );
    if ( cp < mincp ) 
    {
      mincp = cp;
      j = i;
    }
  }
  result = AExitPoints[j];
  return result;
}


bool __fastcall OwnsEnemy( TPoint p )
{
  bool result = false; // чи належить точка ворогу
  result = ! EmptyCell( p ) && !Owns( p );
  return result;
}


bool __fastcall NoEnemiesSurround( int x, int y )
// REQ_0001: Preparation for long step: end of step: don't allow enemy to use your improvement!

{
  bool result = false;
  TPoint a, b;
  int k = 0;
  for ( k = 0; k <= 3; k++)
  {
    a = AStep( x, y, k );
    b = AJump( x, y, k );
    if ( OwnsEnemy( a ) ) 
    {
      result = false;
      return result;
    }
    if ( OwnsEnemy( b ) && !EmptyCell( a ) ) 
    {
      result = false;
      return result;
    }
  }
  result = true;
  return result;
}

bool __fastcall ImprovementExists( TStep Step, TPoint AExitPoints[12])
{
  bool result = false;
  int cx = 0, cy = 0, j = 0, cp = 0, cp0 = 0;
  TPoint NEEP, p, q;
  int PointsLen = 0;
  TPoint Points[ 101/*# range 0..100*/ ];
  if ( Ready( Step.To ) ) 
  {
    result = false;
    return result;
  }
  cx = Step.To.X;
  cy = Step.To.Y;
  AuxiStep.From = Step.To;
  cp0 = QuasiManhattanDistance( Step.From, Step.To );
  result = false;
  if ( abs( cp0 ) == 1 ) 
    return result;

/*    //c = Corners[a];
    CurrentStep.From.X := c.X;
    CurrentStep.From.Y := c.Y;
  Result := -15;*/
  if ( Improve ) 
  {
    PointsLen = 0;
    RecursiveFindAllPossibleJumps( Step.To, &PointsLen, Points );
    NEEP = NearestEmptyExitPoint( Step.To, AExitPoints );
    p = NearestExitPoint( NEEP, Points, PointsLen );
    if ( EmptyCell( p ) ) 
    {
      q = NearestOwnsLongExitPoint( p, Points, PointsLen );
      AuxiStep = MakeStep( q, p, AExitPoints);
    }
    else
      AuxiStep = MakeStep( p, NEEP, AExitPoints);
    result = ! PointsEqual( p, Step.To );//True;    ManhattanDistance()
    return result;
  }
  for ( j = 0; j <= 3; j++)
    if ( ( IsSingleJumpExpected( cx, cy, j ) ) && NoEnemiesSurround( cx, cy ) ) 
//    if ( IsSingleJumpExpected( cx, cy, j ) ) 
    {
      p = AJump( cx, cy, j );
      cp = QuasiManhattanDistance( Step.From, p );
      if ( IsBetter( p, cp, AuxiStep.From, cp0 ) ) 
      {//   (cp > Result)
        cp0 = cp;
        AuxiStep.From.X = p.X;
        AuxiStep.From.Y = p.Y;
        result = true;
      }
    }
  return result;
}


TStep __fastcall AuxilaryStep( TStep Step )
{
  TStep result;
  MaxPrice( AuxiStep.From );
  result = CurrentStep;
  return result;
}


bool __fastcall OwnsExitPoints(TPoint AExitPoints[12])
{
  for (int i = 0; i <= 11; i++)
    if (!(EmptyCell(AExitPoints[i]) || Owns(AExitPoints[i]))) //if ( ! ( EmptyCell( AExitPoints[i] ) | OwnsLong( AExitPoints[i] ) ) ) 
    {
      return false;
    }
  return true;
}


int __fastcall EmptyExitPointCount(TPoint AExitPoints[12])
{
  int result = 0;
  int i = 0;
  result = 0;
  for ( i = 0; i <= 11; i++)
    if ( EmptyCell( AExitPoints[i] ) ) 
      result++;
  return result;
}



bool __fastcall EmptyExitPointExists(TPoint AExitPoints[12])
{
  bool result = false;
  unsigned char i = '\0';
  for ( i = 0; i <= 11; i++)
    if ( EmptyCell( AExitPoints[i] ) ) 
    {
      result = true;
      return result;
    }
  result = false;
  return result;
}


TStep __fastcall FreeExitPoint( TPoint c, TPoint AExitPoints[12])
{
  TStep result;
  TPoint p;
  int d = 0;
  if ( EmptyCell( c ) ) 
    throw "FreeExitPoint(): ExitPoint is already empty!";
  p = NearestEmptyExitPoint( c, AExitPoints, 12);
  if ( ! EmptyCell( p ) ) 
    throw "FreeExitPoint(): No empty exit points!";
  d = ManhattanDistance( c, p );
  switch ( d )
  {
    case 1:
      result = Step( c, p );
    break;
    case 2:
      if ( ( c.X == p.X ) || ( c.Y == p.Y ) ) 
        result = Step( c, p );
      else
        result = FreeExitPoint( Point( c.X, p.Y ), AExitPoints);
    break;
    case 3:
      if ( ( c.X == p.X ) || ( c.Y == p.Y ) ) 
        result = Step( Point( c.X + ( p.X - c.X ) / 3, c.Y + ( p.Y - c.Y ) / 3 ), p );
      else
        result = FreeExitPoint( Point( c.X, p.Y ), AExitPoints);
    break;
  default:
    result = FreeExitPoint( Point( c.X, p.Y ), AExitPoints);
  }
  return result;
}


TStep __fastcall FindBestStep(TPoint ACorners[12], TPoint AExitPoints[12])
{
  TStep result;
  int mp = 0, mpi = 0, i = 0;
  TStep BestStep;
  TPoint NEP;
  mp = - 15;

  // 1! Вивести всі фішки з власного дому.
  // Кожна невиведена фішка має статус - Урджент
  for ( i = 0; i <= 11; i++)
  {
    if ( ( UrgentExists( ACorners ) ) && (!Urgent( ACorners[i] ) )) 
      continue;
    if ( Ready( ACorners[i] ) ) 
      continue;
      //if (Ready(ACorners[i]))
      //  continue;
    if ( OwnsExitPoints( AExitPoints ) & ( EmptyExitPointCount( AExitPoints ) == 1 ) ) 
    {
      NEP = NearestExitPoint( ACorners[i], AExitPoints );
      if ( ! EmptyCell( NEP ) & EmptyExitPointExists( AExitPoints ) ) 
      {
        result = FreeExitPoint( NEP, AExitPoints );
        return result;
      }
      else
      {
        result = MakeStep( ACorners[i], NEP, AExitPoints );
        return result;
      }
    }
    mpi = MaxPrice( ACorners[i] );
      //std::cout << "i: " << i << " mp: " << mp << " Corners[i]: " << Corners[i].X  << Corners[i].Y << " MaxPrice(i): " << mpi << " CurrentStep: " << CurrentStep.From.X << CurrentStep.From.Y << CurrentStep.To.X << CurrentStep.To.Y << std::endl;
    if ( IsBetter( CurrentStep.To, mpi, BestStep.To, mp ) ) 
    {//if (mpi > mp)
      mp = mpi;
      BestStep = CurrentStep;
    }
    if ( mp < 0 ) 
      if ( ImprovementExists( Step( ACorners[i], ACorners[i] ), AExitPoints ) ) 
      {
        if ( Improve ) 
          BestStep = AuxiStep;
        else
          BestStep = AuxilaryStep( Step( ACorners[i], ACorners[i] ) );
        mp = QuasiManhattanDistance( BestStep.From, BestStep.To );
      }
    if ( ( mp < 0 ) & OwnsExitPoints( AExitPoints ) ) 
    {
      NEP = NearestExitPoint( ACorners[i], AExitPoints );
      if ( ! EmptyCell( NEP ) & EmptyExitPointExists( AExitPoints ) ) 
      {
        result = FreeExitPoint( NEP, AExitPoints );
        return result;
      }
    }
    mpi = MaxExpectedPrice( ACorners[i] );
    if ( IsBetter( AuxiStep.To, mpi, BestStep.To, mp ) ) 
    {//if (mpi > mp)
      mp = mpi;
      BestStep = AuxiStep;
    }
  }
  if ( ImprovementExists( BestStep, AExitPoints ) ) 
    if ( Improve ) 
      BestStep = AuxiStep;
    else
      BestStep = AuxilaryStep( BestStep );
  result = BestStep;
  return result;
}



TStep doStep(TPoint ACorners[12], TPoint AExitPoints[12])
{
  TStep result;
  result = FindBestStep(ACorners, AExitPoints);
  /*# with Result do */
  if ( PointsEqual( result.From, result.To ) ) 
    throw "doStep(): PointsEqual(From, To)";
  doMove( result.From.X, result.From.Y, result.To.X, result.To.Y );
  return result;
}

TStep doStep(int sign)
{
  cz = sign;
	if (cz == MAP_YOU)
		return doStep(Corners, FreeSpaces);
	else
		return doStep(OCorners, OFreeSpaces);
  /* TStep result;
  cz = Sign;
  if ( cz == MAP_YOU ) 
    result = doStep( Corners, MAXIDX( Corners ), FreeSpaces, MAXIDX( FreeSpaces ) );
  else
    result = doStep( OCorners, MAXIDX( OCorners ), OFreeSpaces, MAXIDX( OFreeSpaces ) );
  return result;*/

}
;

/*void __fastcall Replay( Textfile& f )
{
  int fromX = 0, fromY = 0, toX = 0, toY = 0;
// DEBUG FEATURES
  YOU_First = true;
  Init( );
  Readln( f, fromX, fromY, toX, toY );
  cz = MAP_YOU;
  doMove( fromX, fromY, toX, toY );
  Readln( f, fromX, fromY, toX, toY );
  cz = MAP_OPONENT;
  doMove( fromX, fromY, toX, toY );
  while ( ! eof( f ) ) 
  {
    Readln( f, fromX, fromY, toX, toY );
    cz = MAP_YOU;
    doMove( fromX, fromY, toX, toY );
    Readln( f, fromX, fromY, toX, toY );
    cz = MAP_OPONENT;
    doMove( fromX, fromY, toX, toY );
  }
// EOF DEBUG FEATURES
}
*/
//void __fastcall Play( Textfile& f )
/*var
  fromX, fromY, toX, toY: Integer;
  step: TStep;*/
//int main(int argc, char* argv[])
/*
{
  cz = MAP_OPONENT;
  move = 1;
  YOU_First = true;
  Init( );
  Writeln( f, move, ' ', Win( Corners ), ' ', Win( OCorners ), ' ' );
/*
    while ((move < 100) and (EpicFail())) do begin
        step := doStep(MAP_YOU);
    Writeln(f, step.From.X, ' ', step.From.Y, ' ', step.To.X, ' ', step.To.Y);
        step := doStep(MAP_OPONENT);
    Writeln(f, step.From.X, ' ', step.From.Y, ' ', step.To.X, ' ', step.To.Y);
        Inc(move);
    end;

(*
        std::cin >> fromX;
        if (std::cin)
        {
            std::cin >> fromY >> toX >> toY;
            cz = MAP_OPONENT;
            YOU_First = false;
            Init();
            doMove(fromX, fromY, toX, toY);
        }
        else
        {
            std::cin.clear();
            std::string s;
            std::cin >> s;
            YOU_First = true;
            Init();
        }
    step = doStep(MAP_YOU);
        std::cout << step.From.X << ' ' << step.From.Y << ' ' << step.To.X << ' ' << step.To.Y << std::endl;
        move++;
*/
/*
    while (isStepPosible())
    {
        std::cin >> fromX;
        if (std::cin)
        {
            std::cin >> fromY >> toX >> toY;
            cz = MAP_OPONENT;
            doMove(fromX, fromY, toX, toY);
        }
        else
        {
            std::cin.clear();
            std::string s;
            std::cin >> s;

        }
        step = doStep(MAP_YOU);
        std::cout << step.From.X << ' ' << step.From.Y << ' ' << step.To.X << ' ' << step.To.Y << std::endl;
        move++;
    }
  return 0;
}*/
//}


void __fastcall InitComp( bool First )
{
  YOU_First = First;
  Init();
}

int main(int argc, char* argv[])
{
    cz = MAP_OPONENT;
	

	int fromX, fromY, toX, toY;
    move = 1;
	TStep step;


// DEBUG FEATURES
/*
	YOU_First = true;
	Init();

    std::cin >> fromX >> fromY >> toX >> toY;
	cz = MAP_YOU;
	doMove(fromX, fromY, toX, toY);
    std::cin >> fromX >> fromY >> toX >> toY;
	cz = MAP_OPONENT;
	doMove(fromX, fromY, toX, toY);

	for (int i; i < 33; i++)
	{
		std::cin >> fromX >> fromY >> toX >> toY;
	    cz = MAP_YOU;
	    doMove(fromX, fromY, toX, toY);
        std::cin >> fromX >> fromY >> toX >> toY;
	    cz = MAP_OPONENT;
	    doMove(fromX, fromY, toX, toY);
		  
	}
*/
// EOF DEBUG FEATURES

	/* YOU_First = true;
			Init();
std::cout << move << " " << Win(Corners) << " " << Win(OCorners) << " "  << std::endl;
	while ((move < 100) && (EpicFail()))
	{
		step = doStep(MAP_YOU);	
		std::cout << step.From.X << " " << step.From.Y << " " << step.To.X << " " << step.To.Y << std::endl;  
			
		step = doStep(MAP_OPONENT);
	    std::cout << step.From.X << " " << step.From.Y << " " << step.To.X << " " << step.To.Y << std::endl;  
		move++;
	}*/
	
	Improve = true;
        std::cin >> fromX;    
		if (std::cin) 
		{      
			std::cin >> fromY >> toX >> toY;
			cz = MAP_OPONENT;
			YOU_First = false;
			Init();
			doMove(fromX, fromY, toX, toY);    
		} 
		else
		{      
			std::cin.clear();      
			std::string s;      
			std::cin >> s; 
			YOU_First = true;
			Init();
			
		}    

		step = doStep(YOU_First?MAP_YOU:MAP_OPONENT);	
	    //step = doStep(YOU_FirstMAP_YOU);	
		std::cout << step.From.X << " " << step.From.Y << " " << step.To.X << " " << step.To.Y << std::endl;  
		move++;

	while (isStepPosible())  
	{    
		std::cin >> fromX;    
		if (std::cin) 
		{      
			std::cin >> fromY >> toX >> toY;
			cz = MAP_OPONENT;
			doMove(fromX, fromY, toX, toY);    
		} 
		else
		{      
			std::cin.clear();      
			std::string s;      
			std::cin >> s;  
			
		}    
		step = doStep(YOU_First?MAP_YOU:MAP_OPONENT);	
	    //step = doStep(MAP_YOU);	
		std::cout << step.From.X << " " << step.From.Y << " " << step.To.X << " " << step.To.Y << std::endl;  
		move++;
	}  
  return 0;
}
