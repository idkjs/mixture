/* Mixture_Monad -- Monadic mixin

   Mixture (https://github.com/michipili/mixture)
   This file is part of Mixture

   Copyright © 2013–2015 Michael Grünewald

   This file must be used under the terms of the CeCILL-B.
   This source file is licensed as described in the file COPYING, which
   you should have received as part of this distribution. The terms
   are also available at
   http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.txt */

module type Basis = {
  type t(+'a);
  let bind: (t('a), 'a => t('b)) => t('b);
  let return: 'a => t('a);
};

module type Methods = {
  type t(+'a);
  let apply: (t('a => 'b), t('a)) => t('b);
  let join: t(t('a)) => t('a);
  let map: ('a => 'b, t('a)) => t('b);
  let bind2: (t('a), t('b), ('a, 'b) => t('c)) => t('c);
  let bind2: (t('a), t('b), ('a, 'b) => t('c)) => t('c);
  let bind3: (t('a), t('b), t('c), ('a, 'b, 'c) => t('d)) => t('d);
  let bind4:
    (t('a), t('b), t('c), t('d), ('a, 'b, 'c, 'd) => t('e)) => t('e);
  let map2: (('a, 'b) => 'c, t('a), t('b)) => t('c);
  let map3: (('a, 'b, 'c) => 'd, t('a), t('b), t('c)) => t('d);
  let map4: (('a, 'b, 'c, 'd) => 'e, t('a), t('b), t('c), t('d)) => t('e);
  let dist: list(t('a)) => t(list('a));
  let ignore: t('a) => t(unit);
  let filter: ('a => t(bool), list(t('a))) => t(list('a));
  let only_if: (bool, t(unit)) => t(unit);
  let unless: (bool, t(unit)) => t(unit);
  let catch: (unit => t('a), exn => t('a)) => t('a);
  module Infix: {
    let (<*>): (t('a => 'b), t('a)) => t('b);
    let (<$>): ('a => 'b, t('a)) => t('b);
    let ( <* ): (t('a), t('b)) => t('a);
    let ( >* ): (t('a), t('b)) => t('b);
    let (>>=): (t('a), 'a => t('b)) => t('b);
    let (>|=): (t('a), 'a => 'b) => t('b);
    let (>>): (t('a), unit => t('b)) => t('b);
    let (>=>): ('a => t('b), 'b => t('c), 'a) => t('c);
    let (<=<): ('b => t('c), 'a => t('b), 'a) => t('c);
  };
};

module Make = (B: Basis) => {
  module Internal = {
    include B;
    let apply = (fm, xm) => bind(fm, f => bind(xm, x => return(f(x))));
  };

  module ApplicativeMethods = Mixture_Applicative.Make(Internal);

  module Infix = {
    let (>>=) = B.bind;

    let (>|=) = (m, f) => B.(m >>= (x => return(f(x))));

    let (>>) = (m, f) => B.bind(m, _ => f());

    let (>=>) = (g, f, x) => B.bind(g(x), f);

    let (<=<) = (f, g, x) => B.bind(g(x), f);

    let (<*>) = ApplicativeMethods.Infix.(<*>);

    let (<$>) = (f, m) => B.(m >>= (x => return(f(x))));

    let ( <* ) = ApplicativeMethods.Infix.( <* );

    let ( >* ) = ApplicativeMethods.Infix.( >* );
  };

  let apply = Internal.apply;

  let join = mm => Infix.(mm >>= (x => x));

  let map = (f, x) => Infix.(f <$> x);

  let map2 = (f, x, y) => Infix.(f <$> x <*> y);

  let map3 = (f, x, y, z) => Infix.(f <$> x <*> y <*> z);

  let map4 = (f, x, y, z, t) => Infix.(f <$> x <*> y <*> z <*> t);

  let bind2 = (m_a, m_b, f) => Infix.(m_a >>= (a => m_b >>= f(a)));

  let bind3 = (m_a, m_b, m_c, f) =>
    Infix.(m_a >>= (a => m_b >>= (b => m_c >>= f(a, b))));

  let bind4 = (m_a, m_b, m_c, m_d, f) =>
    Infix.(m_a >>= (a => m_b >>= (b => m_c >>= (c => m_d >>= f(a, b, c)))));

  let _cons = (hd, tl) => [hd, ...tl];

  let rec dist =
    fun
    | [] => B.return([])
    | [hd, ...tl] => Infix.(_cons <$> hd <*> dist(tl));

  let ignore = m => Infix.(ignore <$> m);

  let filter = (pred, lst) => {
    let rec loop = m =>
      fun
      | [] => Infix.(List.rev <$> m)
      | [hd, ...tl] => Infix.(hd >>= pred >>= maybe_pack(m, hd, tl))
    and maybe_pack = (m, hd, tl, flag) =>
      if (flag) {
        loop(Infix.(_cons <$> hd <*> m), tl);
      } else {
        loop(m, tl);
      };

    loop(B.return([]), lst);
  };

  let only_if = (flag, m) =>
    if (flag) {
      m;
    } else {
      B.return();
    };

  let unless = (flag, m) =>
    if (flag) {
      B.return();
    } else {
      m;
    };

  let catch = (f, h) =>
    try(f()) {
    | exn => h(exn)
    };
};

module type S = {
  type t(+'a);
  include Basis with type t('a) := t('a);
  include Methods with type t('a) := t('a);
};

module Transformer = {
  module type MonadBasis = Basis;
  module type MonadS = S;
  module MonadMake = Make;

  module type S = {
    include MonadS;
    type u(+'a);
    let lift: u('a) => t('a);
  };

  module Make =
         (B: Basis, M: Basis, G: Basis with type t('a) = M.t(B.t('a))) => {
    module MethodsMonad = MonadMake(G);

    include G;
    include MethodsMonad;

    type u('a) = M.t('a);

    let lift = m => M.bind(m, G.return);
  };
};
