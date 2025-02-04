add_rules("mode.debug")

includes("xf_utils/xmake.lua")
includes("xf_sys/xmake.lua")

target("xf_lp")
    set_kind("binary")
    add_cflags("-Wall")
    add_files("examples/*.c")
    add_includedirs("examples")
    add_xf_utils(".")
