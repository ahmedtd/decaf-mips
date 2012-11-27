for full in *.h
do
stripped=$(basename "$full" .h)
echo $stripped".hh"
mv $full $stripped".hh"
done
