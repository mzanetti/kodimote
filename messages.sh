for i in `ls i18n/*.ts`; do
    lupdate -recursive . -ts i18n/$i -no-obsolete
done

lrelease i18n/xbmcremote_*.ts

QRC=i18n/translations.qrc
echo "<RCC>" > $QRC
echo "    <qresource prefix=\"/\">" >> $QRC

for i in `ls i18n/*.qm`; do
    echo "        <file>$i</file>" >> $QRC
done

echo "    </qresource>" >> $QRC
echo "</RCC>" >> $QRC
