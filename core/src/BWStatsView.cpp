#include "../include/BWStatsView.h"
#include "../include/PlayerCommon.h"
#include "../include/PlrFunctionality.h"

using namespace plrCommon;

BWStatsView::BWStatsView()
{
    //ctor
}


BWStatsView::BWStatsView(string s, double posX, double posY, double width, double height, string toolTipText)
{
    reset();
    position  = Vec2d(posX,posY);
    dimension = Vec2d(width,height);
    GUIElement::setToolTipArea(posX,posY,CHANNEL_SIZE_X,CHANNEL_SIZE_Y*8,toolTipText);
}

void BWStatsView::reset()
{
    statisticPoints.resize(AMOUNT_OF_TRAINING_MODES);
    resultSessionBWs.clear();
    resultBaselineBWs.clear();
    resultSessions.clear();
}

BWStatsView::~BWStatsView()
{
    //dtor
}

void BWStatsView::refreshStatistic() {

    // Read all folders from results and grab session bw data's into vector.
    string resultsFolder = BWFolder+"results/";
    path pResultFolderForThisExp;
    string pathStr = "";

    resultSessions.clear();
    resultSessionBWs.clear();
    for (int i=0;i<AMOUNT_OF_TRAINING_MODES;i++)
        statisticPoints[i].clear();

    for (int i=1; i<1001; i++)
    {
        pathStr = resultsFolder+intToStr(i)+"/";
        pResultFolderForThisExp = path(pathStr);
        if (!exists (pResultFolderForThisExp)) // This indexed folder doesn't exists. So we'll have this one!
            break;
        else {

            // BW Data..
            BWManager bwTemp = getBWManagerOfFile(pathStr+"session.bwr");
            elementList.unLinkElementById(bwTemp.getId()); // UNLINK IMMEDIATELY from GUISystem!
            resultSessionBWs.push_back(getBWManagerOfFile(pathStr+"session.bwr"));

            // Session data.
            Session tmpSes;
            elementList.unLinkElementById(tmpSes.getId());
            playerLog(pathStr+"session.dat");
            if ( boost::filesystem::exists( (pathStr+"session.dat").c_str() ))
            {
                std::ifstream ifs( (pathStr+"session.dat").c_str()  );
                boost::archive::text_iarchive ia(ifs);
                ia >> tmpSes;
            }
            resultSessions.push_back(tmpSes);

        }
    }

    // Gathering!
    int resultMode = STAT_RELATIVE;
    double divider = 1.0;
    if (resultModeRadioButton.getSelectionTxt()==resultModeRadioButtonOp1.getText()) {
        resultMode = STAT_RELATIVE;
        divider = 1.0;
    } else {
        resultMode = STAT_POWER;
        divider = 100.0;
    }

    // Gather for statistic view of different session types.
    for (int i=0;i<resultSessions.size();i++) {
        if (resultSessions[i].trainingType==GAMMA_MEDITATION_TRAINING) {
            statisticPoints[GAMMA_MEDITATION_TRAINING].push_back(resultSessionBWs[i].getSpecificOfResult("gamma",AVG_PHASE,resultMode)/divider);
            playerLog("Added gamma avarage of session.");
        }
        if (resultSessions[i].trainingType==THETA_ALPHA_MEDITATION_TRAINING) {
            statisticPoints[THETA_ALPHA_MEDITATION_TRAINING].push_back(resultSessionBWs[i].getSpecificOfResult("theta+alfa",AVG_PHASE,resultMode)/divider);
            playerLog("Added theta/alfa avarage of session.");
        }
    }


    /* Gather experiment results for analysing and save to file. */
    string resultStr = "#id|Session type|g1(A)|g2(A)|g3(A)|gAvg(A)|gΔ(A)|g1(r)|g2(r)|g3(r)|gAvg(r)|gΔ(r)|t+a1(A)|t+a2(A)|t+a3(A)|t+aAvg(A)|t+aΔ(A)|t+a1(r)|t+a2(r)|t+a3(r)|t+aAvg(r)|t+aΔ(r)|t/a1(A)|t/a2(A)|t/a3(A)|t/aAvg(A)|t/aΔ(A)|t/a1(r)|t/a2(r)|t/a3(r)|t/aAvg(r)|t/aΔ(r)";
    /*string gammaStr = "#id|Session type|g3(A)|gAvg(A)|gΔ(A)|g3(r)|gAvg(r)|gΔ(r)|t+a1(A)|t+a2(A)|t+a3(A)|t+aAvg(A)|t+aΔ(A)|t+a1(r)|t+a2(r)|t+a3(r)|t+aAvg(r)|t+aΔ(r)|t/a1(A)|t/a2(A)|t/a3(A)|t/aAvg(A)|t/aΔ(A)|t/a1(r)|t/a2(r)|t/a3(r)|t/aAvg(r)|t/aΔ(r)";
    string taStr = "#id|Session type|gΔ(A)|gΔ(r)|t+aΔ(A)|t+aΔ(r)|t/aΔ(A)|t/aΔ(r)|";*/
    string gammaStr = resultStr;
    string taStr = resultStr;
    for (int i=0;i<resultSessions.size();i++) {
        string tmpStr = "";
        tmpStr += "\n"+intToStr(i)+"|";
        string prefix="";
        if (resultSessions[i].trainingType==GAMMA_MEDITATION_TRAINING)
            prefix += "Gamma-session";
        if (resultSessions[i].trainingType==THETA_ALPHA_MEDITATION_TRAINING)
            prefix += "Theta/Alfa-session";

            tmpStr += prefix;

            tmpStr += "|"+desToStr(resultSessionBWs[i].getSpecificOfResult("gamma",START_PHASE,STAT_POWER));
            tmpStr += "|"+desToStr(resultSessionBWs[i].getSpecificOfResult("gamma",MIDDLE_PHASE,STAT_POWER));
            tmpStr += "|"+desToStr(resultSessionBWs[i].getSpecificOfResult("gamma",END_PHASE,STAT_POWER));
            tmpStr += "|"+desToStr(resultSessionBWs[i].getSpecificOfResult("gamma",AVG_PHASE,STAT_POWER));
            tmpStr += "|"+desToStr(resultSessionBWs[i].getSpecificOfResult("gamma",END_PHASE,STAT_POWER)-resultSessionBWs[i].getSpecificOfResult("gamma",START_PHASE,STAT_POWER));

            tmpStr += "|"+desToStr(resultSessionBWs[i].getSpecificOfResult("gamma",START_PHASE,STAT_RELATIVE));
            tmpStr += "|"+desToStr(resultSessionBWs[i].getSpecificOfResult("gamma",MIDDLE_PHASE,STAT_RELATIVE));
            tmpStr += "|"+desToStr(resultSessionBWs[i].getSpecificOfResult("gamma",END_PHASE,STAT_RELATIVE));
            tmpStr += "|"+desToStr(resultSessionBWs[i].getSpecificOfResult("gamma",AVG_PHASE,STAT_RELATIVE));
            tmpStr += "|"+desToStr(resultSessionBWs[i].getSpecificOfResult("gamma",END_PHASE,STAT_RELATIVE)-resultSessionBWs[i].getSpecificOfResult("gamma",START_PHASE,STAT_RELATIVE));

            tmpStr += "|"+desToStr(resultSessionBWs[i].getSpecificOfResult("t+a",START_PHASE,STAT_POWER));
            tmpStr += "|"+desToStr(resultSessionBWs[i].getSpecificOfResult("t+a",MIDDLE_PHASE,STAT_POWER));
            tmpStr += "|"+desToStr(resultSessionBWs[i].getSpecificOfResult("t+a",END_PHASE,STAT_POWER));
            tmpStr += "|"+desToStr(resultSessionBWs[i].getSpecificOfResult("t+a",AVG_PHASE,STAT_POWER));
            tmpStr += "|"+desToStr(resultSessionBWs[i].getSpecificOfResult("t+a",END_PHASE,STAT_POWER)-resultSessionBWs[i].getSpecificOfResult("t+a",START_PHASE,STAT_POWER));

            tmpStr += "|"+desToStr(resultSessionBWs[i].getSpecificOfResult("t+a",START_PHASE,STAT_RELATIVE));
            tmpStr += "|"+desToStr(resultSessionBWs[i].getSpecificOfResult("t+a",MIDDLE_PHASE,STAT_RELATIVE));
            tmpStr += "|"+desToStr(resultSessionBWs[i].getSpecificOfResult("t+a",END_PHASE,STAT_RELATIVE));
            tmpStr += "|"+desToStr(resultSessionBWs[i].getSpecificOfResult("t+a",AVG_PHASE,STAT_RELATIVE));
            tmpStr += "|"+desToStr(resultSessionBWs[i].getSpecificOfResult("t+a",END_PHASE,STAT_RELATIVE)-resultSessionBWs[i].getSpecificOfResult("t+a",START_PHASE,STAT_RELATIVE));

            tmpStr += "|"+desToStr(resultSessionBWs[i].getSpecificOfResult("t/a",START_PHASE,STAT_POWER));
            tmpStr += "|"+desToStr(resultSessionBWs[i].getSpecificOfResult("t/a",MIDDLE_PHASE,STAT_POWER));
            tmpStr += "|"+desToStr(resultSessionBWs[i].getSpecificOfResult("t/a",END_PHASE,STAT_POWER));
            tmpStr += "|"+desToStr(resultSessionBWs[i].getSpecificOfResult("t/a",AVG_PHASE,STAT_POWER));
            tmpStr += "|"+desToStr(resultSessionBWs[i].getSpecificOfResult("t/a",END_PHASE,STAT_POWER)-resultSessionBWs[i].getSpecificOfResult("t/a",START_PHASE,STAT_POWER));

            tmpStr += "|"+desToStr(resultSessionBWs[i].getSpecificOfResult("t/a",START_PHASE,STAT_RELATIVE));
            tmpStr += "|"+desToStr(resultSessionBWs[i].getSpecificOfResult("t/a",MIDDLE_PHASE,STAT_RELATIVE));
            tmpStr += "|"+desToStr(resultSessionBWs[i].getSpecificOfResult("t/a",END_PHASE,STAT_RELATIVE));
            tmpStr += "|"+desToStr(resultSessionBWs[i].getSpecificOfResult("t/a",AVG_PHASE,STAT_RELATIVE));
            tmpStr += "|"+desToStr(resultSessionBWs[i].getSpecificOfResult("t/a",END_PHASE,STAT_RELATIVE)-resultSessionBWs[i].getSpecificOfResult("t/a",START_PHASE,STAT_RELATIVE));

        //gΔ(A)|gΔ(r)|t+aΔ(A)|t+aΔ(r)|t/aΔ(A)|t/aΔ(r)
        // Group printing..
 /*       string tmpStr = "\n"+intToStr(i)+"|"+prefix;
        tmpStr += "|"+desToStr(resultSessionBWs[i].getSpecificOfResult("gamma",END_PHASE,STAT_POWER)-resultSessionBWs[i].getSpecificOfResult("gamma",START_PHASE,STAT_POWER));
        tmpStr += "|"+desToStr(resultSessionBWs[i].getSpecificOfResult("gamma",END_PHASE,STAT_RELATIVE)-resultSessionBWs[i].getSpecificOfResult("gamma",START_PHASE,STAT_RELATIVE));
        tmpStr += "|"+desToStr(resultSessionBWs[i].getSpecificOfResult("t+a",END_PHASE,STAT_POWER)-resultSessionBWs[i].getSpecificOfResult("t+a",START_PHASE,STAT_POWER));
        tmpStr += "|"+desToStr(resultSessionBWs[i].getSpecificOfResult("t+a",END_PHASE,STAT_RELATIVE)-resultSessionBWs[i].getSpecificOfResult("t+a",START_PHASE,STAT_RELATIVE));
        tmpStr += "|"+desToStr(resultSessionBWs[i].getSpecificOfResult("t/a",END_PHASE,STAT_POWER)-resultSessionBWs[i].getSpecificOfResult("t/a",START_PHASE,STAT_POWER));
        tmpStr += "|"+desToStr(resultSessionBWs[i].getSpecificOfResult("t/a",END_PHASE,STAT_RELATIVE)-resultSessionBWs[i].getSpecificOfResult("t/a",START_PHASE,STAT_RELATIVE));
*/
        if (resultSessions[i].trainingType==GAMMA_MEDITATION_TRAINING)
            gammaStr += tmpStr;
        if (resultSessions[i].trainingType==THETA_ALPHA_MEDITATION_TRAINING)
            taStr += tmpStr;

        resultStr += tmpStr;

    }

    // Combine
    resultStr += "\n" + gammaStr + "\n" + taStr;

    // Change all dots to , for helping later math processing in external programs ie libre office calc spread sheet.
    replace( resultStr.begin(), resultStr.end(), '.', ',' );

    // Yes u heard what I said.. save to file.. Cmon.. Save it..:)
    std::ofstream out("renderComplex.txt");
    out << resultStr;
    out.close();

    /*
    START_PHASE,
       MIDDLE_PHASE,
       END_PHASE,
    AVG_PHASE,
    */

    //playerLog("searching this..");

    // Do whatever with the data.

    for (int i=0;i<statisticPoints[GAMMA_MEDITATION_TRAINING].size();i++)
        playerLog("found gamma ses " + intToStr(i+1) + " with gamma value " + desToStr(statisticPoints[GAMMA_MEDITATION_TRAINING][i]));
    for (int i=0;i<statisticPoints[THETA_ALPHA_MEDITATION_TRAINING].size();i++)
        playerLog("found theta+alfa " + intToStr(i+1) + " with t+a value " + desToStr(statisticPoints[THETA_ALPHA_MEDITATION_TRAINING][i] ));

   /* playerLog("------------------------ ");
    playerLog("Gamma group g_g_sum " + desToStr(g_g_sum) + " g_ta_sum " + desToStr(g_ta_sum));
    playerLog("Theta/Alfa -group ta_ta_sum " + desToStr(ta_ta_sum) + " ta_g_sum " + desToStr(ta_g_sum));
    playerLog("------------------------ ");
*/
}

bool BWStatsView::mouseInside() {
    if (guiMouse.getMouseClickX() > position.x() && guiMouse.getMouseClickY() > position.y() && guiMouse.getMouseClickX() < position.x() + dimension.x() && guiMouse.getMouseClickY() < position.y() + dimension.y())
        return true;
    else
        return false;
            return true;
}

bool BWStatsView::mouseClickedInside() {
    if (mouseInside()) {
        if (guiMouse.getLeftButtonState()) {
            return true;
        }
    }
}


/*
* refresh()
*::::::::
*
* To refresh all the channels data. Usually this is done in one second interval. Here also is calculated the avarage channel's value of all graphs(dominating freq).
*
*/
void BWStatsView::refresh()
{

    // Check for interaction If it's visible.
    if (isVisible())
    {
        //.. Draw something so it's possible to see how the data starts to look like :)

    }
}

void BWStatsView::drawStatsGraph(int statListIndex, int yGraphIndex, int amountOfGraphs, ALLEGRO_COLOR useColor, string descTxt) {
    // Draw statistic
    yGraphIndex -= 1;
    double xInc = dimension.x()/(statisticPoints[statListIndex].size()-1);
    double gH = dimension.y()/amountOfGraphs; // Graph height.
    double centerY = position.y() + gH*yGraphIndex+gH/2;

    al_draw_text(libraryFont,al_map_rgba_f(1,1,1,0.8), position.x()+10, centerY-10, 0, descTxt.c_str());

    // 2.0 max, 1.0 middle, 0.0 is zero
    al_draw_line(position.x(), centerY, position.x()+dimension.x(), centerY, al_map_rgba_f(1, 1, 1, 0.3), 1.0); // Border.
    al_draw_line(position.x(), centerY+gH/2, position.x()+dimension.x(), centerY+gH/2, al_map_rgba_f(0, 0, 1, 0.3), 1.0); // Border.
    al_draw_line(position.x(), centerY-gH/2, position.x()+dimension.x(), centerY-gH/2, al_map_rgba_f(0, 1, 0, 0.3), 1.0); // Border.

    for (int i=1;i<statisticPoints[statListIndex].size();i++) {
        double sPPrev = statisticPoints[statListIndex][i-1];
        double sPThis = statisticPoints[statListIndex][i];
        al_draw_line(position.x()+(i-1)*xInc, centerY-sPPrev*(gH/2)+gH/2, position.x()+i*xInc, centerY-sPThis*(gH/2)+gH/2, useColor, 3.0);
        //string text = (intToStr(i)+".");
        double pDx = -7;
        double pDy = -13;
        if (i==1)
            al_draw_text(libraryFont,al_map_rgba_f(0,1.2,0.8,0.8),  position.x()+(i-1)*xInc+pDx+10, centerY-sPPrev*(gH/2)+gH/2+pDy, 0, (intToStr(i)+".").c_str());
        al_draw_text(libraryFont,al_map_rgba_f(0,1.2,0.8,0.8),  position.x()+i*xInc+pDx, centerY-sPThis*(gH/2)+gH/2+pDy, 0, (intToStr(i+1)+".").c_str());
    }
}

/*
* draw()
*::::::::
*
* :p
*
*/
void BWStatsView::draw()
{
    //  Draw graph lines to show graph changes.
    if (isVisible())
    {
        if (!showVisualization.isChecked())
            drawScaledPicture(graphBaseImg,position.x(),position.y(),dimension.x(),dimension.y());

    // Upwards is downwards as usual coordinates here <3
        al_draw_rectangle(position.x(), position.y(), position.x()+dimension.x(), position.y()+dimension.y(), al_map_rgba_f(1, 1, 1, 0.5), 3.0f);

        if (statisticPoints[GAMMA_MEDITATION_TRAINING].size() > 0)
            drawStatsGraph(GAMMA_MEDITATION_TRAINING        , 1, 2, getGraphThemeColorOf(CHANNEL_GAMMA) , "Gamma meditation.");
        if (statisticPoints[THETA_ALPHA_MEDITATION_TRAINING].size() > 0)
            drawStatsGraph(THETA_ALPHA_MEDITATION_TRAINING  , 2, 2, getGraphThemeColorOf(CHANNEL_ALPHA) , "Theta/Alpha meditation.");

    }
    GUIElement::draw();
}
