/* Mixture_Applicative -- Applicatives

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
  let return: 'a => t('a);
  let apply: (t('a => 'b), t('a)) => t('b);
};

module type Methods = {
  type t(+'a);
  let map: ('a => 'b, t('a)) => t('b);
  let map2: (('a, 'b) => 'c, t('a), t('b)) => t('c);
  let map3: (('a, 'b, 'c) => 'd, t('a), t('b), t('c)) => t('d);
  let map4: (('a, 'b, 'c, 'd) => 'e, t('a), t('b), t('c), t('d)) => t('e);
  let dist: list(t('a)) => t(list('a));
  module Infix: {
    let (<*>): (t('a => 'b), t('a)) => t('b);
    let (<$>): ('a => 'b, t('a)) => t('b);
    let ( <* ): (t('a), t('b)) => t('a);
    let ( >* ): (t('a), t('b)) => t('b);
  };
};

module Make = (B: Basis) => {
  open B;

  module Infix = {
    let (<*>) = apply;

    let (<$>) = (f, x) => apply(return(f), x);

    let ( <* ) = (m_a, m_b) => ((x, _) => x) <$> m_a <*> m_b;

    let ( >* ) = (m_a, m_b) => ((_, y) => y) <$> m_a <*> m_b;
  };

  let map = (f, x) => Infix.(f <$> x);

  let map2 = (f, x, y) => Infix.(f <$> x <*> y);

  let map3 = (f, x, y, z) => Infix.(f <$> x <*> y <*> z);

  let map4 = (f, x, y, z, t) => Infix.(f <$> x <*> y <*> z <*> t);

  let rec dist = {
    let cons = (hd, tl) => [hd, ...tl];

    fun
    | [] => return([])
    | [hd, ...tl] => Infix.(cons <$> hd <*> dist(tl));
  };
};

module type S = {
  type t(+'a);
  include Basis with type t('a) := t('a);
  include Methods with type t('a) := t('a);
};
