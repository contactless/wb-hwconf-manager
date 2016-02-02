eval "`slot_get_vars $SLOT`"
local SLOT_TYPE=${SLOT%%[0-9]}
local SLOT_NUM=${SLOT#$SLOT_TYPE}

CONFIG_GPIO=${CONFIG_GPIO:-/etc/wb-homa-gpio.conf}

# Add GPIO to the wb-homa-gpio driver config
# Args:
# - gpio name (for mqtt)
# - gpio number (as in /sys/class/gpio)
# - direction - "input" or "output"
# These 3 args can be repeated multiple times to add many gpios at once
wb_gpio_add() {
	local JSON=$CONFIG_GPIO
	local items=()
	while [[ $# -ge 3 ]]; do
		[[ -n "$1" && -n "$2" && -n "$3" ]] || {
			die "Bad arguments"
			return 1
		}
		items+=( "{name: \"$1\", gpio: $2, direction: \"$3\"}" )
		shift 3
	done

	json_array_append ".channels" "${items[@]}"
}

# Remove GPIO from the wb-homa-gpio driver config
# Args:
# - gpio number (as in /sys/class/gpio)
wb_gpio_del() {
	local JSON=$CONFIG_GPIO
	json_array_delete ".channels" \
		". as \$chan | ([$(join ', ' "$@")] | map(. == \$chan.gpio) | any)"
}

wb_max_slot_num() {
	jq '
		[ .slots[].id | select(startswith("'$1'")) ] |
		map(ltrimstr("'$1'") | tonumber) |
		max
	' "$CONFIG"
}
