add_rules("mode.debug", "mode.release")

add_requires("boost 1.85.0")

target("BasicHttpServer")
    set_kind("binary")
    add_files("src/**.cpp")
    add_headerfiles("src/**.h")
    add_includedirs("src")
    add_links("wsock32", "ws2_32")
    add_packages("boost")
    add_defines("NOMINMAX")