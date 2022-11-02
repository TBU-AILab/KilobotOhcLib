//
// Created by Peter Janků on 02.11.2022.
//

#ifndef KILOBOTOHCLIB_KILOBOTOVERHEADCONTROLLER_H
#define KILOBOTOHCLIB_KILOBOTOVERHEADCONTROLLER_H

#include <QObject>

namespace KilobotOhcLib {

    class KilobotOverheadController: public QObject {
        Q_OBJECT
        public:
            explicit KilobotOverheadController(QObject *parent = 0);
            ~KilobotOverheadController();
    };

} // KilobotOhcLib

#endif //KILOBOTOHCLIB_KILOBOTOVERHEADCONTROLLER_H
