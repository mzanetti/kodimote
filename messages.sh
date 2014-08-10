tx pull -a

lupdate -recursive . -ts i18n/xbmcremote.ts -no-obsolete

lrelease i18n/xbmcremote_*.ts

cd i18n

QRC=translations.qrc
echo "<RCC>" > $QRC
echo "    <qresource prefix=\"/\">" >> $QRC

for i in `ls *.qm`; do
    echo "        <file>$i</file>" >> $QRC
done

echo "    </qresource>" >> $QRC
echo "</RCC>" >> $QRC
