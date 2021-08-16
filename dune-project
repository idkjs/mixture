(lang dune 2.9)

(generate_opam_files true)

(package
 (name mixture)
 (depends
  ;; General system dependencies
  (dune
   (>= 2.2))
  (ocaml
   (>= 4.10.0))
  broken
  (reason
   (and
    (>= 3.6.0)
    (< 4.0.0)))
  ;; Dev dependencies
  (ocaml-lsp-server :dev)))
