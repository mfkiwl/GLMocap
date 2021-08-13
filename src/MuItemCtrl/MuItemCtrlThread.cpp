#include "MuItemCtrlThread.h"

MuItemCtrlThread::MuItemCtrlThread(MuItemCtrl *muItemCtrl)
{
    this->muItemCtrl = muItemCtrl;
}

void MuItemCtrlThread::run()
{
    OPENVIO *vio;
    int i, ii;

    mlog->show("MuItemCtrlThread start");
    
    vioList = muItemCtrl->vioList;
    idlist.clear();

    mlog->show("All Device:");
    for (i = 0; i < vioList->size(); i++)
    {
        mlog->show(QString(vioList->at(i)->idShort));
        vioList->at(i)->setStatus("WAIT Upgrade");
        idlist.append(QString(vioList->at(i)->idStr));
    }

    for (i = 0; i < idlist.size(); i++)
    {
        vio = nullptr;
        for(ii=0;ii<vioList->size();ii++)
        {
            if(idlist.at(i).contains(QString(vioList->at(ii)->idStr)))
            {
                vio = vioList->at(ii);
                break;
            }
        }
        
        if(vio != nullptr)
        {
            if (muItemCtrl->type == CTRL_TYPE_UPGRADE)
            {
                upgrade(vio);
            }
        }

    }

    mlog->show("MuItemCtrlThread exit");
}

int MuItemCtrlThread::upgrade(OPENVIO *vio)
{
    int waitCount = 3000;
    mlog->show("upgrade " + QString(vio->idShort) + " start");

    muItemCtrl->firmwareUpgrade->setOPENVIO(vio);
    muItemCtrl->firmwareUpgrade->setBinPath(muItemCtrl->binPath);
    muItemCtrl->firmwareUpgrade->upgradeStart();

    while(muItemCtrl->firmwareUpgrade->state != FirmwareUpgrade::STATE_SUCCESS && waitCount > 0)
    {
        msleep(10);
        waitCount--;
    }

    if(waitCount <= 0)
    {
        mlog->show("upgrade " + QString(vio->idShort) + " timeout");
    }
    mlog->show("upgrade " + QString(vio->idShort) + " exit");
}