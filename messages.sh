tx pull -a

lupdate -recursive . -ts i18n/kodimote.ts -no-obsolete

lrelease i18n/kodimote_*.ts

cd i18n

QRC=translations.qrc
echo "<RCC>" > $QRC
echo "    <qresource prefix=\"/\">" >> $QRC

for i in `ls *.qm`; do
    echo "        <file>$i</file>" >> $QRC
done

echo "    </qresource>" >> $QRC
echo "</RCC>" >> $QRC
