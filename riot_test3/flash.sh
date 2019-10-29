HEXFILE=./bin/*/*.hex

# echo using "${HEXFILE}"
# echo using $HEXFILE

OPENOCD_CMD=$(printf 'program "%s" verify reset exit' $HEXFILE)

case $1 in

    stlink)
           ST-LINK_CLI.exe -c SWD -P $HEXFILE -V -Rst
        ;;

    openocd)
        openocd -f stlink_v2.cfg -c "${OPENOCD_CMD}"
        ;;

    *)
        echo "usage: flash.sh stlink | openocd"
        ;;
esac

